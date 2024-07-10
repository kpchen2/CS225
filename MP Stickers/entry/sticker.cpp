#include "Image.h"
#include "StickerSheet.h"

int main() {
  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //

  Image alma; alma.readFromFile("../data/alma.png");
  Image i;    i.readFromFile("../data/i.png");
  Image expected; expected.readFromFile("../data/expected.png");
  Image expected2; expected2.readFromFile("../data/expected-2.png");

  StickerSheet s1(alma, 5);
  s1.addSticker(i, 20, 200);
  s1.addSticker(i, 40, 200);

  StickerSheet s2(s1);
  s2.removeSticker(1);

  s1.render().writeToFile("TEST_IMAGE.png");
  // s2.render().writeToFile("TEST_IMAGE.png");

  return 0;
}
