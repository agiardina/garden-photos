#include <unordered_map>
#include <wx/wx.h>
#include <wx/sizer.h>
#include "photos.h"


#ifndef _IMAGE_PANEL_H_
#define _IMAGE_PANEL_H_

class wxImagePanel : public wxScrolledWindow
{
private:
  int w, h;
  int vscroll = 0;
  wxTimer m_timer;
  std::vector<photo> photos;
  std::unordered_map<std::string, wxBitmap*> cache;
  std::unordered_map<std::string, int> cache_x;
  std::unordered_map<std::string, int> cache_y;
  std::unordered_map<std::string, int> cache_w;
  std::unordered_map<std::string, int> cache_h;  
  std::unordered_map<std::string, int> cache_res;
  
  void calcVirtualSize();  
  
public:
  wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);
    
  void paintEvent(wxPaintEvent & evt);
  void paintNow();
  void OnSize(wxSizeEvent& event);
  void OnScroll(wxScrollWinEvent& event);
  void OnTimer(wxTimerEvent& event);  
  void render(wxDC& dc);
  void setPhotos(std::vector<photo> photos);
  void increaseResolution();

  std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();

  DECLARE_EVENT_TABLE()
};


#endif  // _IMAGE_PANEL_H_
