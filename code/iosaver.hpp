#ifndef IOSAVER_HPP_
#define IOSAVER_HPP_

#include <ios>
#include <locale>

class iosaver
{
public:
  iosaver(std::basic_ios<char>& stream)
  : stream_(stream),
    exceptions_(stream_.exceptions()),
    fill_(stream_.fill()),
    flags_(stream_.flags()),
    locale_(stream.getloc()),
    precision_(stream_.precision()),
    state_(stream.rdstate()),
    width_(stream.width())
  {}
  ~iosaver()
  {
    stream_.exceptions(exceptions_);
    stream_.fill(fill_);
    stream_.flags(flags_);
    stream_.imbue(locale_);
    stream_.precision(precision_);
    stream_.setstate(state_);
    stream_.width(width_);
  }
private:
  std::basic_ios<char>& stream_;
  std::ios_base::iostate exceptions_;
  char fill_;
  std::ios_base::fmtflags flags_;
  std::locale locale_;
  std::streamsize precision_;
  std::ios_base::iostate state_;
  std::streamsize width_;
};

#endif
