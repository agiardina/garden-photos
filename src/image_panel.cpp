#include "image_panel.h"
#include "photos.h"
#include <list>
#include <unordered_set>
#include <wx/filefn.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/wx.h>
#include <wx/event.h>

#define TIMER_ID 100
#define LOW_RES 0
#define HIGH_RES 1

BEGIN_EVENT_TABLE(image_panel, wxScrolledWindow)
EVT_SCROLLWIN(image_panel::OnScroll)
EVT_TIMER(TIMER_ID, image_panel::OnTimer)
END_EVENT_TABLE()

image_panel::image_panel(wxFrame* parent)
: wxScrolledWindow(parent)
, m_timer(this, TIMER_ID)
{
    w = -1;
    h = -1;
    SetScrollRate(1, 1);
    m_timer.Start(300);
    SetBackgroundColour(wxColour(16,16,16));
}

void image_panel::init(config &cfg, Poco::Data::Session &session)
{
    m_cfg = &cfg;
    m_session = &session;
    m_thumbs_path = m_cfg->thumbs_path();
    load_photos();
    bind_events();
}

int image_panel::get_client_height()
{
    int neww, newh;
    GetClientSize(&neww, &newh);
    return newh;
}

int image_panel::max_scroll()
{
    return std::max(0, virtual_height - get_client_height());
}

int image_panel::id_at_xy(int x, int y)
{
    int i_id = 0;
    for(auto& it : cache) {
        std::string id = it.first;
        int x_start = cache_x[id];
        int x_end = cache_x[id] + cache_w[id];
        int y_start = cache_y[id];
        int y_end = cache_y[id] + cache_h[id];
        
        if (x >= x_start && x <= x_end
            && y >= y_start && y <= y_end) {
            i_id = std::stoi(id);
            break;
        }
    }
    return i_id;
}

int image_panel::box_n_at_xy(int x, int y)
{
    int X = x;
    int Y = vscroll + y;
    int n_X = (X / box_size);
    int n_Y = (Y / box_size);
    return (n_Y * n_cols) + n_X;
}

void image_panel::OnTimer(wxTimerEvent& event)
{
    increase_resolution();
}

void image_panel::OnScroll(wxScrollWinEvent& evt)
{
    repaint = true;
    int step = 8;
    if(wxEVT_SCROLLWIN_LINEUP == evt.GetEventType() && vscroll > step) {
        vscroll = vscroll - step;
    } else if(wxEVT_SCROLLWIN_LINEDOWN == evt.GetEventType()) {
        vscroll = vscroll + step;
    } else {
        vscroll = evt.GetPosition();
    }
    
    if(vscroll > max_scroll()) {
        vscroll = max_scroll();
    }
    
    m_timer.Stop();
    Scroll(0, vscroll);
    Refresh();
    m_timer.Start(300);
}

void image_panel::increase_resolution()
{
    bool refresh = false;
    for(auto& it : cache) {
        if(cache_res.at(it.first) == LOW_RES) {
            wxImage image;
            wxBitmap* resized;
            std::string id = it.first;
            std::string filename = m_cfg->thumbs_path() + "/960/" + id + ".jpg";
            if(wxFileExists(filename) && image.LoadFile(filename, wxBITMAP_TYPE_JPEG)) {
                refresh = true;
                resized = new wxBitmap(image.Scale(cache_w[id]*dpi, cache_h[id]*dpi, wxIMAGE_QUALITY_HIGH), -1, dpi);
                delete cache[id];
                cache[id] = resized;
                cache_res[id] = HIGH_RES;
            }
        }
    }
    
    if(refresh) Refresh();
}

void image_panel::render_photo(wxDC& dc, const std::string &photo_path)
{
    wxImage image;
    int client_w, client_h, img_w, img_h, w, h, max_size;
    
    GetClientSize(&client_w, &client_h);
    if(wxFileExists(photo_path) && image.LoadFile(photo_path, wxBITMAP_TYPE_JPEG)) {
        img_w = image.GetWidth();
        img_h = image.GetHeight();

        std::tie(w,h) = photos::calc_width_height(img_w,img_h,client_w,client_h);
        wxBitmap resized(image.Scale(w*dpi, h*dpi, wxIMAGE_QUALITY_NEAREST), -1, dpi);
        dc.DrawBitmap(resized, (int)((client_w-w)/2), (int)((client_h-h)/2));
    }
}

void image_panel::render_photos(wxDC& dc, std::vector<photos::photo> photos)
{
    if (box_size > 0) {
        
        int neww, newh;
        GetClientSize(&neww, &newh);
        int n_visible_rows = (newh / box_size) + 2;
        int start_row = vscroll / box_size;
        int voffset = vscroll % box_size;
        std::unordered_set<std::string> render_set;
        
        std::cout << "Photos Size " << photos.size()  << "\n";
        if(photos.size() > 0) {
            for(int row = start_row; row < start_row + n_visible_rows; row++) {
                for(int col = 0; col < n_cols; col++) {
                    
                    int left, top;
                    int img_width;
                    int img_height;
                    int new_img_width;
                    int new_img_height;
                    double ratio;
                    wxImage image;
                    std::string id;
                    unsigned long i = (row * n_cols) + col;
                    
                    if(i < photos.size()) {
                        id = std::to_string(photos[i].id);
                        render_set.insert(id);
            
                        std::string filename = m_cfg->thumbs_path() + "/320/" + id + ".jpg";
                        if(wxFileExists(filename) && image.LoadFile(filename, wxBITMAP_TYPE_JPEG)) {
                            
                            img_width = image.GetWidth();
                            img_height = image.GetHeight();
                            ratio = (double)img_width / (double)img_height;
                            
                            if(img_width > img_height) {
                                new_img_width = img_size;
                                new_img_height = new_img_width / ratio;
                            } else {
                                new_img_height = img_size;
                                new_img_width = ratio * new_img_height;
                            }
                            left = (box_size - new_img_width) / 2;
                            top = (box_size - new_img_height) / 2;
                            
                            wxBitmap* resized;
                            if(cache.find(id) == cache.end()) {
                                cache_w[id] = new_img_width;
                                cache_h[id] = new_img_height;
                                cache_res[id] = LOW_RES;
                                // resized = new
                                // wxBitmap(image.Scale(new_img_width*dpi,new_img_height*dpi,wxIMAGE_QUALITY_HIGH),-1,dpi);
                                resized = new wxBitmap(
                                                       image.Scale(cache_w[id]*dpi, cache_h[id]*dpi, wxIMAGE_QUALITY_NEAREST), -1, dpi);
                                cache[id] = resized;
                            }
                            
                            cache_x[id] = (col * box_size) + left;
                            cache_y[id] = ((row - start_row) * box_size) + top - voffset;
                            
                            w = neww;
                            h = newh;
                            dc.DrawBitmap(*cache[id], cache_x[id], cache_y[id], false);
                        } else {
                            // std::cout << "File not foud " << id << "\n";
                        }
                    }
                }
            }
        }
        
        
        // Cleaning up memory for out of view images
        std::list<std::unordered_map<std::string, wxBitmap*>::const_iterator> itrs;
        for(auto x = cache.cbegin(); x != cache.cend(); x++) {
            if(render_set.count(x->first) == 0) {
                itrs.push_back(x);
            }
        }
        for(auto it : itrs) {
            delete it->second;
            cache.erase(it);
        }
    }
}

void image_panel::clean_cache()
{
    for(auto x = cache.cbegin(); x != cache.cend(); x++) {
        delete x->second;
    }
    cache.clear();
    cache_x.clear();
    cache_y.clear();
    cache_w.clear();
    cache_h.clear();
    cache_res.clear();
}

void image_panel::set_virtual_size(int n_photos)
{
    
    int neww, newh;
    GetClientSize(&neww, &newh);
    virtual_width = neww;
    box_size = neww / n_cols;
    img_size = box_size - 5;
    n_rows = n_photos / n_cols;
    
    if(n_photos > 0) {
        if(n_photos % n_cols != 0) {
            n_rows++;
        }
            
        virtual_height = n_rows * box_size;
        if(vscroll > max_scroll()) {
            vscroll = max_scroll();
            Scroll(0, vscroll);
        }
        SetVirtualSize(virtual_width, virtual_height);
    }
}

void image_panel::reset()
{
    m_photo_mode = false;
    m_photo_id = 0;
    m_photo_n = 0;
    set_virtual_size(m_photos.size());
    clean_cache();
    Refresh();
}

void image_panel::bind_events()
{
    Bind(wxEVT_PAINT, &image_panel::on_paint, this);
    Bind(wxEVT_LEFT_DCLICK, &image_panel::on_dbl_click, this);
    Bind(wxEVT_KEY_DOWN, &image_panel::on_key_down, this);
    Bind(wxEVT_SIZE,&image_panel::on_size, this);
}

void image_panel::on_paint(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    if (m_photo_mode) {
        render_photo(dc, m_photo_path);
    } else {
        render_photos(dc, m_photos);
    }
}

void image_panel::on_dbl_click(wxMouseEvent& evt)
{
    m_photo_id  = id_at_xy(evt.m_x, evt.m_y);
    m_photo_n = box_n_at_xy(evt.m_x, evt.m_y);
    m_photo_mode = true;
    
    m_photo_path = m_thumbs_path + "/960/" + std::to_string(m_photo_id) + ".jpg";
    Refresh();
}

void image_panel::photo_mode()
{
    m_photo_mode = false;
    Refresh();
}

void image_panel::next_photo()
{
    if (m_photo_n < m_photos.size()-1) {
        photos::photo photo = m_photos.at(m_photo_n+1);
        m_photo_n += 1;
        m_photo_id = photo.id;
        m_photo_path = m_thumbs_path + "/960/" + std::to_string(m_photo_id) + ".jpg";
        Refresh();
    }
}

void image_panel::prev_photo()
{
    if (m_photo_n > 0) {
        photos::photo photo = m_photos.at(m_photo_n-1);
        m_photo_n -= 1;
        m_photo_id = photo.id;
        m_photo_path = m_thumbs_path + "/960/" + std::to_string(m_photo_id) + ".jpg";
        Refresh();
    }
}

void image_panel::load_photos()
{
    std::vector<photos::photo>().swap(m_photos);
    m_photos = photos::all_photos(*m_session);
    reset();
}

void image_panel::load_favorites_photos()
{
    std::vector<photos::photo>().swap(m_photos);
    m_photos = photos::favorites_photos(*m_session);
    reset();
}

void image_panel::on_key_down(wxKeyEvent& evt)
{
    int key = evt.GetKeyCode();
    
    if (key == 27) { //Esc
        photo_mode();
    } else if (key == 316) { // Right arrow
        next_photo();
    } else if (key == 314) { // Left Arrow
        prev_photo();
    }
}

void image_panel::on_size(wxSizeEvent& evt)
{
    set_virtual_size(m_photos.size());
    clean_cache();
    Refresh();
    evt.Skip();
}
