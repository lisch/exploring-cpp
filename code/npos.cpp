class string
{
public:
  typedef int size_type;
  static size_type const npos = -1;
};

string::size_type string::npos;

int main()
{
  string s;
  return string::npos;
}
