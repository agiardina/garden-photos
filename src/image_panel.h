#include <unordered_map>
#include <wx/event.h>
#include <wx/wx.h>
#include <wx/sizer.h>
#include "config.h"
#include "photos.h"

#ifndef _IMAGE_PANEL_H_
#define _IMAGE_PANEL_H_

wxDECLARE_EVENT(GP_PHOTO_CHANGED, wxCommandEvent);

class garden_photos;

class image_panel : public wxScrolledWindow
{
private:
    config* m_cfg;
    std::vector<photos::photo> m_photos;
    bool m_photo_mode = false;
    int m_photo_id = 0;
    int m_photo_n = 0;
    std::string m_photo_path;
    std::string m_thumbs_path;
    Poco::Data::Session *m_session;
    
    bool repaint = true;
    double dpi = 2.0;
    int w, h;
    int vscroll = 0;
    int virtual_height = 0;
    int virtual_width = 0;
    int n_cols = 5;
    int n_rows = 0;
    int box_size = 1;
    int img_size = 0;
    wxTimer m_timer;
    std::unordered_map<std::string, wxBitmap*> cache;
    std::unordered_map<std::string, int> cache_x;
    std::unordered_map<std::string, int> cache_y;
    std::unordered_map<std::string, int> cache_w;
    std::unordered_map<std::string, int> cache_h;
    std::unordered_map<std::string, int> cache_res;
    int get_client_height();
    int max_scroll();
    int photo_id(int id);
    void bind_events();
  
public:
    image_panel(wxFrame* parent);
    void init(config &cfg, Poco::Data::Session &session);
    void OnScroll(wxScrollWinEvent& event);
    void OnTimer(wxTimerEvent& event);
    void clean_cache();
    void render_photos(wxDC& dc, const std::vector<photos::photo> photos);
    void render_photo(wxDC& dc, const std::string &photo_path);
    int  id_at_xy(int x, int y);
    int  box_n_at_xy(int x, int y);
    void set_virtual_size(int n_photos);
    void increase_resolution();
    void show_photo(std::string path);
    void on_paint(wxPaintEvent& evt);
    void on_dbl_click(wxMouseEvent& evt);
    void on_key_down(wxKeyEvent& evt);
    void on_size(wxSizeEvent& evt);
    void next_photo();
    void prev_photo();
    void photo_mode();
    void load_photos();
    void load_favorites_photos();
    void reset();
    int displayed_photo();
    
    std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
    DECLARE_EVENT_TABLE()
};


#endif  // _IMAGE_PANEL_H_
