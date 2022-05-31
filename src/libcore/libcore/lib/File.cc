#include <fstream>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/File.hh>

namespace Engine {
std::string readFile(std::string_view path) {
  std::ifstream is;
  is.open(path.data(), std::ios::ate);
  assert(is.is_open());

  size_t size = is.tellg();
  std::string buf(size, 0);

  is.seekg(0);
  is.read(buf.data(), size);
  is.close();

  return buf;
}
} // namespace Engine