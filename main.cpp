#include "cmd_line/cmd_line.h"
#include <visp/vpImageIo.h>
#include <visp/vpVideoReader.h>
#include <visp/vpVideoWriter.h>
#include <visp/vpImage.h>
#include <visp/vpRGBa.h>
#include <visp/vpDisplay.h>
#include <vector>
#include <visp/vpDisplayX.h>

#define DEBUG_WRITE(S) if(cmd.get_debug()) std::cout << S  << std::endl
#define CAPTION_OFFSET_X 20
#define CAPTION_OFFSET_Y 20

int main(int argc,char** argv){
  CmdLine cmd(argc,argv);
  DEBUG_WRITE("Starting program");
  vpDisplay* disp = new vpDisplayX();

  vpVideoWriter writer;
  vpImage<vpRGBa> out_I,out_I_overlay;
  std::vector< vpImage<vpRGBa> > in_I;
  if(cmd.should_exit()) return 0; //exit if needed

  DEBUG_WRITE("initializing writer");
  writer.setFileName((cmd.get_output_dir() + std::string("/%08d.jpg")).c_str());
  writer.open(out_I);

  std::vector<boost::shared_ptr<VideoWithCaption> > videos = cmd.get_videos();
  unsigned int cursor_x=0,cursor_y=0;

  DEBUG_WRITE("initializing images");

  for(std::vector<boost::shared_ptr<VideoWithCaption> >::iterator i=videos.begin();i!=videos.end();i++){
    if(cmd.get_debug())
      std::cout << "initializing image from " << (*i)->caption << "(" << (*i)->path <<")"<< std::endl;
    (*i)->reader.acquire((*i)->I);
    (*i)->y=cursor_y;
    (*i)->x=cursor_x;
    cursor_x+=(*i)->I.getWidth();
    if(cmd.get_debug())
      std::cout << "configuring a new image at " << (*i)->x << "," << (*i)->y << std::endl;
  }

  out_I.resize(std::max(cursor_y,videos[0]->I.getHeight()),cursor_x);
  disp->init(out_I);
  if(cmd.get_debug())
    std::cout << "Output image size set to " << out_I.getWidth() << "x" << out_I.getHeight() << std::endl;
  bool last_frame_reached=false;
  for(;!last_frame_reached;){
    vpDisplay::display(out_I);
    for(std::vector<boost::shared_ptr<VideoWithCaption> >::iterator i=videos.begin();i!=videos.end();i++){
      if((*i)->reader.getFrameIndex()-2>(*i)->reader.getLastFrameIndex()){
        last_frame_reached=true;
        break;
      }
      out_I.insert((*i)->I,vpImagePoint((*i)->y,(*i)->x));
      vpDisplay::displayCharString(out_I,vpImagePoint((*i)->y+CAPTION_OFFSET_X,(*i)->x+CAPTION_OFFSET_Y),(*i)->caption.c_str(),vpColor::blue);
      (*i)->reader.acquire((*i)->I);
    }
    vpDisplay::flush(out_I);
    vpDisplay::getImage(out_I,out_I_overlay);
    writer.saveFrame(out_I_overlay);
    writer.saveFrame(out_I_overlay);
    writer.saveFrame(out_I_overlay);
  }
  writer.close();

  return 0;
}
