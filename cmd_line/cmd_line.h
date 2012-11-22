#ifndef __CMD_LINE_H__
#define __CMD_LINE_H__
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>

#include <string>
#include <vector>
#include <visp/vpVideoReader.h>
#include <visp/vpRGBa.h>
#include <boost/shared_ptr.hpp>
namespace po = boost::program_options;

struct VideoWithCaption{
  std::string caption;
  std::string path;
  vpVideoReader reader;
  vpImage<vpRGBa> I;
  unsigned int x;
  unsigned int y;
};

class CmdLine{
 private:
  po::options_description prog_args;
  boost::program_options::variables_map vm_;
  bool should_exit_;
  std::vector<std::string> dirs_;
  std::vector<std::string> captions_;
  std::string output_dir_;
  std::vector<boost::shared_ptr<VideoWithCaption> > videos_;
 public:
  CmdLine(int argc,char**argv);

  std::vector<boost::shared_ptr<VideoWithCaption> >& get_videos();
  std::string& get_output_dir();
  bool should_exit();
  bool get_verbose();
  bool get_debug();
};
#endif

