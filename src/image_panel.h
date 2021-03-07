#include <unordered_map>
#include <wx/wx.h>
#include <wx/sizer.h>
#include "photos.h"
#include "dispatcher.h"


#ifndef _IMAGE_PANEL_H_
#define _IMAGE_PANEL_H_

class image_panel : public wxScrolledWindow
{
private:
  bool repaint = true;
  
  double dpi = 2.0;
  int w, h;
  int vscroll = 0;
  int virtual_height = 0;
  int virtual_width = 0;  
  int n_cols = 5;
  int n_rows = 0;
  int box_size = 0;
  int img_size = 0;
  
  wxTimer m_timer;
  std::vector<photos::photo> m_photos;
  std::unordered_map<std::string, wxBitmap*> cache;
  std::unordered_map<std::string, int> cache_x;
  std::unordered_map<std::string, int> cache_y;
  std::unordered_map<std::string, int> cache_w;
  std::unordered_map<std::string, int> cache_h;  
  std::unordered_map<std::string, int> cache_res;
  
  void calcVirtualSize();
  void cleanCache();
  int getClientHeight();
  int maxScroll();
  
public:
  image_panel(wxFrame* parent);
    
  void paintEvent(wxPaintEvent & evt);
  void paintNow();
  void OnSize(wxSizeEvent& event);
  void OnScroll(wxScrollWinEvent& event);
  void OnTimer(wxTimerEvent& event);
  void OnMouseEvents(wxMouseEvent& event);
  void render(wxDC& dc);
  void setPhotos(std::vector<photos::photo> photos);
  void increaseResolution();

  std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
  dispatcher *m_dispatcher;

  DECLARE_EVENT_TABLE()
};


#endif  // _IMAGE_PANEL_H_
