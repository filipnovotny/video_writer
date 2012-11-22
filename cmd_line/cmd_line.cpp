#include "cmd_line.h"


#define DEBUG_WRITE(S) if(get_debug()) std::cout << S  << std::endl

CmdLine::CmdLine(int argc,char**argv) : should_exit_(false) {
      po::options_description general("General options");

      general.add_options()
          ("directories,d", po::value< std::vector<std::string> >(&dirs_),"directories to display on video")
          ("captions,c", po::value< std::vector<std::string> >(&captions_),"captions to display on videos (in the same order as directories)")
          ("output-directory,o", po::value<std::string>(&output_dir_),"output directory where the concatenated images are stored")
          ("verbose,v", "verbose mode")
          ("debug,D", "debug mode")

          ("help", "produce help message")
          ;

      prog_args.add(general);
      po::store(po::parse_command_line(argc, argv, prog_args), vm_);
      po::notify(vm_);

      if (vm_.count("help")) {
          std::cout << prog_args << std::endl;
          should_exit_ = true;
      }
      DEBUG_WRITE("Debug mode active.");

      unsigned int nbvid=0;
      std::string cur_caption("");
      for(std::vector< std::string >::iterator i=dirs_.begin(), cap_iter=captions_.begin();
          i!=dirs_.end();
          i++,nbvid++){
        if(cap_iter==captions_.end())
          cur_caption = "";
        else{
          cur_caption = *cap_iter;
          cap_iter++;
        }
        boost::shared_ptr<VideoWithCaption> el(new VideoWithCaption());
        el->caption=cur_caption;
        el->path = i->c_str();
        el->reader.setFileName( i->c_str() );
        el->reader.setFirstFrameIndex(0);
        el->reader.open(el->I);
        videos_.push_back(el);
        if(get_verbose()){
          std::cout << "Adding video with path " << el->path << " and caption " << el->caption << std::endl;
        }
      }
      DEBUG_WRITE("Done adding videos.");

      if(get_verbose())
        std::cout << nbvid << " videos added." << std::endl;
}

bool CmdLine::should_exit(){
  return should_exit_;
}


std::vector<boost::shared_ptr<VideoWithCaption> >& CmdLine::get_videos(){

  return videos_;
}

std::string& CmdLine::get_output_dir(){
  return output_dir_;
}

bool CmdLine::get_verbose(){
  return vm_.count("verbose")>0;
}

bool CmdLine::get_debug(){
  return vm_.count("debug")>0;
}
