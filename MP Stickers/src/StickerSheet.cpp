#include "StickerSheet.h"
#include "Image.h"
#include <vector>
#include <iostream>

StickerSheet::StickerSheet(const Image& picture, unsigned max) {
  // Image im = picture;
  // im_ = &im;
  im_ = new Image(picture);
  max_ = max;
  num_ = 0;
  im_width_ = picture.width();
  im_height_ = picture.height();

  for (unsigned i = 0; i < max; i++) {
    vecptr_.push_back(nullptr);
    xpos_.push_back(0);
    ypos_.push_back(0);
  }
  // vecptr_.resize(max);
  // xpos_.resize(max);
  // ypos_.resize(max);
}

StickerSheet::StickerSheet(const StickerSheet& other) {
  // Image im = *other.im_;
  // im_ = &im;
  // delete im_;
  im_ = new Image(*other.im_);
  im_width_ = other.im_width_;
  im_height_ = other.im_height_;

  vecptr_.clear();
  xpos_.clear();
  ypos_.clear();

  for (unsigned i = 0; i < other.max_; i++) {
    // Image im = *other.vecptr_.at(i);
    // vecptr_.push_back(&im);
    vecptr_.push_back(other.vecptr_.at(i));
    int x = other.xpos_.at(i);
    xpos_.push_back(x);
    int y = other.ypos_.at(i);
    ypos_.push_back(y);
  }

  max_ = other.max_;
  num_ = other.num_;
}

StickerSheet::~StickerSheet() {
  delete im_;
}

const StickerSheet& StickerSheet::operator=(const StickerSheet& other) {
  // Image im = *other.im_;
  // im_ = &im;
  delete im_;
  im_ = new Image(*other.im_);
  im_width_ = other.im_width_;
  im_height_ = other.im_height_;

  vecptr_.clear();
  xpos_.clear();
  ypos_.clear();

  for (unsigned i = 0; i < other.max_; i++) {
    // Image im = *other.vecptr_.at(i);
    // vecptr_.push_back(&im);
    vecptr_.push_back(other.vecptr_.at(i));
    int x = other.xpos_.at(i);
    xpos_.push_back(x);
    int y = other.ypos_.at(i);
    ypos_.push_back(y);
  }

  max_ = other.max_;
  num_ = other.num_;

  return *this;
}

void StickerSheet::changeMaxStickers(unsigned max) {
  std::vector<Image*> vec;
  std::vector<int> x;
  std::vector<int> y;
  // max_ = max;
  num_ = 0;

  if (max < max_) {
    for (unsigned i = 0; i < max; i++) {
      vec.push_back(vecptr_.at(i));
      x.push_back(xpos_.at(i));
      y.push_back(ypos_.at(i));
      
      if (vecptr_.at(i) != nullptr) {
        num_++;
      }
    }
    vecptr_ = vec;
    xpos_ = x;
    ypos_ = y;
  } else {
    vecptr_.resize(max);
    xpos_.resize(max);
    ypos_.resize(max);
  }
  max_ = max;
}

int StickerSheet::addSticker(Image& sticker, int x, int y) {
  unsigned count = 0;
  bool placed = false;

  for (unsigned i = 0; i < max_; i++) {
    if (vecptr_.at(i) == nullptr) {
      vecptr_.at(i) = &sticker;
      xpos_.at(i) = x;
      ypos_.at(i) = y;
      placed = true;
      i = max_+1;
    } else {
      count++;
    }
  }

  if (!placed) {
    vecptr_.push_back(&sticker);
    xpos_.push_back(x);
    ypos_.push_back(y);
    max_++;
  }
  num_++;
  return count;
}

int StickerSheet::setStickerAtLayer(Image& sticker, unsigned layer, int x, int y) {
  if (layer >= max_ || layer < 0) {
      return -1;
  }
  
  if (vecptr_.at(layer) == nullptr) {
    num_++;
  }

  vecptr_.at(layer) = &sticker;
  xpos_.at(layer) = x;
  ypos_.at(layer) = y;

  return layer;
}

bool StickerSheet::translate(unsigned index, int x, int y) {
  if (index >= max_ || index < 0 || vecptr_.at(index) == nullptr) {
      return false;
  }
  xpos_.at(index) = x;
  ypos_.at(index) = y;

  return true;
}

void StickerSheet::removeSticker(unsigned index) {
  if (index < max_ && index >= 0) {
    if (vecptr_.at(index) != nullptr) {
      vecptr_.at(index) = nullptr;
      xpos_.at(index) = 0;
      ypos_.at(index) = 0;
      num_--;
    }
  }
}

Image* StickerSheet::getSticker(unsigned index) {
  if (index >= max_ || index < 0) {
      return NULL;
  }
  return vecptr_.at(index);
}


int StickerSheet::layers() const {
  return max_;
}

Image StickerSheet::render() const {
  int left = 0;
  int right = im_width_;
  int top = 0;
  int bot = im_height_;

  for (unsigned i = 0; i < max_; i++) {
      if (vecptr_.at(i) != nullptr) {
          if (xpos_.at(i) < left) {
              left = xpos_.at(i);
          }
          if (xpos_.at(i) + (int)vecptr_.at(i)->width() > right) {
              right = xpos_.at(i) + (int)vecptr_.at(i)->width();
          }
          if (ypos_.at(i) < top) {
              top = ypos_.at(i);
          }
          if (ypos_.at(i) + (int)vecptr_.at(i)->height() > bot) {
              bot = ypos_.at(i) + (int)vecptr_.at(i)->height();
          }
      }
  }

  int x_offset = -left;
  int y_offset = -top;
  
  Image image(right-left, bot-top);

  for (unsigned i = 0; i < im_width_; i++) {
      for (unsigned j = 0; j < im_height_; j++) {
          image.getPixel(i+x_offset,j+y_offset) = im_->getPixel(i,j);
      }
  }

  for (unsigned index = 0; index < max_; index++) {
    if (vecptr_.at(index) != nullptr) {
      for (unsigned i = 0; i < vecptr_.at(index)->width(); i++) {
        for (unsigned j = 0; j < vecptr_.at(index)->height(); j++) {
          if (vecptr_.at(index)->getPixel(i,j).a != 0) {
            image.getPixel(i + x_offset + xpos_.at(index), j + y_offset + ypos_.at(index)) = vecptr_.at(index)->getPixel(i,j);
          }
        }
      }
    }
  }

  // for (auto a = vecptr_.begin(); a != vecptr_.end(); ++a) {
  //     if (*a != nullptr) {
  //         for (unsigned i = 0; i < (*a)->width(); i++) {
  //             for (unsigned j = 0; j < (*a)->height(); j++) {
  //                 image.getPixel(i+x_offset+xpos_.at(i),j+y_offset+ypos_.at(i)) = (*a)->getPixel(i,j);
  //             }
  //         }
  //     }
  // }
  return image;
}