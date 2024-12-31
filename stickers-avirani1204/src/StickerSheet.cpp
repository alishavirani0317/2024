#include "StickerSheet.h"
StickerSheet::StickerSheet(const Image &picture) {
    baseImage = picture;
    // maxStickers_ = 0; // no stickers been added yet
}

int StickerSheet::addSticker(Image& sticker, int x, int y) {
    for (unsigned i = 0; i < stickers.size(); ++i) {
        if (stickers[i] == nullptr) {
            stickers[i] = &sticker;
            xCoords[i] = x;
            yCoords[i] = y;
            return i;
        }
    }
    stickers.push_back(&sticker);
    xCoords.push_back(x);
    yCoords.push_back(y);
    return stickers.size() - 1;
}

int StickerSheet::setStickerAtLayer(Image& sticker, unsigned layer, int x, int y) {

    // std::cout << "Requested layer: " << layer << std::endl;
    // std::cout << "Current stickers size: " << stickers.size() << std::endl;
    // std::cout << "Current xCoords size: " << xCoords.size() << std::endl;
    // std::cout << "Current yCoords size: " << yCoords.size() << std::endl;

    //making sure vector is big enough for requested layer
     if (stickers.size() <= layer) {
        stickers.push_back(&sticker); // add a new "empty" layer
        xCoords.push_back(x); // add default x coord
        yCoords.push_back(y); //add default y coord
        return stickers.size() - 1;
     } else {
        stickers[layer] = &sticker;
        xCoords[layer] = x;
        yCoords[layer] = y;
    // std::cout << "Set sticker at layer: " << layer << ", with coordinates: (" << x << ", " << y << ")" << std::endl;

     return layer;
     }
     // check if there is already a sticker at this later, delete it 
    //  if (stickers[layer] != nullptr) {
    //     stickers[layer] = nullptr;
    //  }

    // std::cout << "After resizing, stickers size: " << stickers.size() << std::endl;

     //set new sticker and its coordinates
    //  stickers[layer] = &sticker;
    //  xCoords[layer] = x;
    //  yCoords[layer] = y;
    // // std::cout << "Set sticker at layer: " << layer << ", with coordinates: (" << x << ", " << y << ")" << std::endl;

    //  return layer;
}

bool StickerSheet::translate(unsigned index, int x, int y) {
    if (index >= stickers.size() || stickers[index] == nullptr) {
        return false;
    }
    xCoords[index] = x;
    yCoords[index] = y;
    return true;
}

void StickerSheet::removeSticker(unsigned index) {
    if (index < stickers.size()) {
        stickers[index] = nullptr;
    }
}
Image* StickerSheet::getSticker(unsigned index) {
    if (index >= stickers.size() || stickers[index] == nullptr) {
        return nullptr;
    }
    return stickers[index];
}

int StickerSheet::layers() const {
    return stickers.size();
}

Image StickerSheet::render() const {
    //get the required hieght and width of the final image
    int maxX = baseImage.width();
    int maxY = baseImage.height();
    int minX = 0;
    int minY = 0;
    // go through stickers to adjust width & height if needed
    for(unsigned i = 0; i < stickers.size(); ++i) {
        if (stickers[i] != nullptr) {
            int stickerRightEdge = xCoords[i] + stickers[i]->width();
            int stickerBotEdge = yCoords[i] + stickers[i]->height();
            int stickerLeftEdge = xCoords[i];
            int stickerTopEdge = yCoords[i];
            //change max w and h to fit all stickers 
            if(stickerLeftEdge < minX) {
                minX = stickerLeftEdge;
            }
            if (stickerRightEdge > maxX) {
                maxX = stickerRightEdge;
            }
            if (stickerTopEdge < minY) {
                minY = stickerTopEdge;
            }
            if (stickerBotEdge > maxY) {
                maxY = stickerBotEdge;
            }
            //Track min x & y coordinates for stickers that r positioned negatively
            // if(xCoords[i] < minX) {
            //     minX = xCoords[i];
            // }
            // if (yCoords[i] < minY) {
            //     minY = yCoords[i];
            // }
        }
    }
    //adjust final dimensions to account for stickers w neg coords
    int finalW = maxX - minX;
    int finalH = maxY - minY;
    //make a new image of the required size
    Image output(finalW, finalH);
    // Copy base image pixels to the final image
    for (unsigned x = 0; x < baseImage.width(); ++x) {
        for (unsigned y = 0; y < baseImage.height(); ++y) {
            output.getPixel(x - minX,y - minY) = baseImage.getPixel(x,y);
        }
    }
    //overlay stickers on top of base image
    for (unsigned i = 0; i < stickers.size(); ++i) {
        if (stickers[i] != nullptr) {
            for (unsigned x = 0; x < stickers[i]->width(); ++x) {
                for (unsigned y = 0; y < stickers[i]->height(); ++y) {
                    // HSLAPixel &stickerPixel = stickers[i].getPixel(x,y);
                    int outputX = xCoords[i] + x - minX;
                    int outputY = yCoords[i] + y - minY;
                    // if (outputX < finalW && outputY < finalH) {
                        if (stickers[i]->getPixel(x,y).a != 0) {
                            output.getPixel(outputX, outputY) = stickers[i]->getPixel(x,y);
                        }
                    // }
                }
            }
        }
    }
    return output;
}