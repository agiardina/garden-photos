#include <unordered_set>
#include <list>
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/filefn.h>
#include <wx/settings.h>
#include "image_panel.h"
#include "photos.h"

#define TIMER_ID 100

BEGIN_EVENT_TABLE(wxImagePanel, wxScrolledWindow)
EVT_PAINT(wxImagePanel::paintEvent)
EVT_SIZE(wxImagePanel::OnSize)
EVT_SCROLLWIN(wxImagePanel::OnScroll)
EVT_TIMER(TIMER_ID, wxImagePanel::OnTimer)
END_EVENT_TABLE()

wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) :
wxScrolledWindow(parent),
  m_timer(this, TIMER_ID)
{
  w = -1;
  h = -1;
  SetScrollRate(1, 1);
  m_timer.Start(300);
  SetBackgroundColour(*wxWHITE);
}

void wxImagePanel::paintEvent(wxPaintEvent & evt)
{
  wxPaintDC dc(this);
  render(dc);
}
void wxImagePanel::OnTimer(wxTimerEvent& event)
{
  increaseResolution();
}

void wxImagePanel::OnScroll(wxScrollWinEvent & evt)
{
  int step = 8;
  if (wxEVT_SCROLLWIN_LINEUP == evt.GetEventType() && vscroll > step) {
    vscroll=vscroll-step;
  } else if (wxEVT_SCROLLWIN_LINEDOWN == evt.GetEventType()) {
    vscroll=vscroll+step;
  } else {
    vscroll = evt.GetPosition();    
  }
  m_timer.Stop();
  Scroll(0,vscroll);  
  Refresh();
  m_timer.Start(300);
}

void wxImagePanel::paintNow()
{
  // depending on your system you may need to look at double-buffered dcs
  wxClientDC dc(this);
  render(dc);
}

void wxImagePanel::increaseResolution()
{
  for (auto& it: cache) {
    if (cache_res.at(it.first) == 60) {
      wxImage image;      
      wxBitmap* resized;
      std::string id = it.first;
      std::string filename = "/Users/agiardina/agallery/thumbs/320/" +  id + ".jpg";
      if (wxFileExists(filename) && image.LoadFile(filename, wxBITMAP_TYPE_JPEG)) {
	resized = new wxBitmap(image.Scale(cache_w[id],cache_h[id],wxIMAGE_QUALITY_HIGH),-1,2.0);
	delete cache[id];
	cache[id] = resized;
	cache_res[id] = 320;
      }
    }
  }
  Refresh();
}

void wxImagePanel::render(wxDC&  dc)
{
  int neww, newh;
  GetClientSize(&neww,&newh);
  int n_visible_rows = (newh / box_size) + 2;
  double dpi = 2.0;
  int start_row = vscroll / box_size;
  int voffset = vscroll % box_size;
  std::unordered_set<std::string> render_set;

  //SetVirtualSize(400,box_size*photos.size()/n_cols);

  //    if (w != neww || h != newh) {
  if (photos.size() > 0) {
    for (int row=start_row;row<start_row+n_visible_rows;row++) {
      for (int col=0;col<n_cols;col++) {
	unsigned long i = 0;
	if (i<photos.size()) {
	  int left, top;
	  int img_width;
	  int img_height;
	  int new_img_width;
	  int new_img_height;
	  double ratio;
	  wxImage image;
	  std::string id;
	  
	  i = (row*n_cols)+col;
	  id = std::to_string(photos[i].id);
	  render_set.insert(id);

	  std::string filename = "/Users/agiardina/agallery/thumbs/320/" +  id + ".jpg";
	  if (wxFileExists(filename) && image.LoadFile(filename, wxBITMAP_TYPE_JPEG)) {
		
	    img_width = image.GetWidth();
	    img_height = image.GetHeight();
	    ratio = (double)img_width / (double)img_height;

	    if (img_width > img_height) {
	      new_img_width = img_size;
	      new_img_height = new_img_width / ratio;
	    } else {
	      new_img_height = img_size;
	      new_img_width = ratio * new_img_height;
 	    }
	    left = (box_size - new_img_width) / 2;
	    top  = (box_size - new_img_height) / 2;

	    wxBitmap* resized;
	    if (cache.find(id) == cache.end()) {
	      cache_w[id] = new_img_width*dpi;
	      cache_h[id] = new_img_height*dpi;
	      cache_res[id] = 60;	  
	      //resized = new wxBitmap(image.Scale(new_img_width*dpi,new_img_height*dpi,wxIMAGE_QUALITY_HIGH),-1,dpi);
	      resized = new wxBitmap(image.Scale(cache_w[id],cache_h[id],wxIMAGE_QUALITY_NEAREST),-1,dpi);
	      cache[id] = resized;
	    }

	    cache_x[id] = (col*box_size)+left;
	    cache_y[id] = ((row-start_row)*box_size)+top-voffset;

	    w = neww;
	    h = newh;
	    dc.DrawBitmap(*cache[id], cache_x[id], cache_y[id], false );
	  } else {
	    std::cout << "File not foud " << id << "\n";
	  }
	}
      }
    }
  }
  //}
  
  //Cleaning up memory for out of view images
  std::list<std::unordered_map<std::string,wxBitmap*>::const_iterator> itrs;
  for (auto x = cache.cbegin(); x != cache.cend(); x++) {
    if (render_set.count(x->first) == 0) {
      itrs.push_back(x);
    }
  }
  for (auto it: itrs) {
    delete it->second;
    cache.erase(it);
  }
}

void wxImagePanel::calcVirtualSize()
{
  if (photos.size()) {
    int neww, newh;
    GetClientSize( &neww, &newh);
    virtual_width = neww;    
    box_size = neww / n_cols;
    img_size = box_size - 5;
    n_rows = photos.size() / n_cols;
    if (photos.size() % n_cols != 0) n_rows++;
    virtual_height = n_rows * box_size;
    SetVirtualSize(virtual_width,virtual_height);
  }
}

void wxImagePanel::OnSize(wxSizeEvent& event)
{
  calcVirtualSize();
  cache.clear();
  Refresh();
  event.Skip();
}

void wxImagePanel::setPhotos(std::vector<photo> v_photos)
{
  photos = v_photos;
  calcVirtualSize();
}
