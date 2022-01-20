#include "edits.hpp"

#include <cstdlib>
#include <iostream>

wxImage editor::makeSquared(const wxBitmap& bitmap, const wxRect region, const wxColour* bgColor, int offsetX, int offsetY, int margin) {
    wxBitmap subBitmap = bitmap.GetSubBitmap(region);
    int size = subBitmap.GetWidth() > subBitmap.GetHeight() ? subBitmap.GetWidth() : subBitmap.GetHeight();
    size += margin * 2;

    //Good ol' C memory operations
    unsigned char *color = (unsigned char*)malloc(sizeof(unsigned char) * 3);
    color[0] = bgColor->Red();
    color[1] = bgColor->Green();
    color[2] = bgColor->Blue();
    
    unsigned char *rgbData = (unsigned char*)malloc(sizeof(unsigned char) * size * size * 3);
    for (int i = 0; i < size*size; i++) {
        memcpy(rgbData + sizeof(unsigned char) * 3 * i,
               color,
               sizeof(unsigned char) * 3
        );
    }

    free(color);
    color = NULL;

    wxBitmap result = wxImage(size, size, rgbData); //the wxImage takes ownership of the data
    wxMemoryDC dc(result);

    wxMemoryDC mdc;
    mdc.SelectObjectAsSource(subBitmap);

    int dstX = subBitmap.GetWidth() > subBitmap.GetHeight() ? margin + offsetX : (subBitmap.GetHeight() - subBitmap.GetWidth()) / 2 + offsetX + margin;
    int dstY = subBitmap.GetWidth() < subBitmap.GetHeight() ? margin + offsetY : (subBitmap.GetWidth() - subBitmap.GetHeight()) / 2 + offsetY + margin;
    dc.Blit(dstX, dstY, bitmap.GetWidth(), bitmap.GetHeight(), &mdc, 0, 0, wxCOPY);

    return result.ConvertToImage();
}

wxImage editor::makeSquaredCrop (const wxImage& img, int offset) {
    bool wide = img.GetWidth() > img.GetHeight();
    int x = wide ? (img.GetWidth() - img.GetHeight()) / 2 : 0;
    int y = !wide ? (img.GetHeight() - img.GetWidth()) / 2 : 0;
    int size = wide ? img.GetHeight() : img.GetWidth();
    
    wxRect region(x + (wide ? offset : 0), y + (wide ? 0 : offset), size, size);

    wxImage result = img.GetSubImage(region);
    return result;
}

wxImage editor::makeSquaredBlur (const wxImage& img, const int radius) {
    wxImage result = img.Copy();
    
    double scale = img.GetWidth() > img.GetHeight()
                    ? (double)img.GetWidth() / img.GetHeight()
                    : (double)img.GetHeight() / img.GetWidth();

    result = result.Scale(img.GetWidth() * scale, img.GetHeight() * scale);
    
    int radii[3];
    double sigma = (2*radius+1) / 6.0;
    boxesForGauss(radii, sigma, 3);

    result = result.Blur(radii[0]);
    result = result.Blur(radii[1]);
    result = result.Blur(radii[2]);

    result = makeSquaredCrop(result);

    wxBitmap result_bitmap(result); // This is slow
    wxMemoryDC dc(result_bitmap);
    
    wxBitmap bitmap(img); // This is slow
    wxMemoryDC mdc;
    mdc.SelectObjectAsSource(bitmap);

    int dstX = img.GetWidth() > img.GetHeight() ? 0 : (img.GetHeight() - img.GetWidth()) / 2;
    int dstY = img.GetWidth() < img.GetHeight() ? 0 : (img.GetWidth() - img.GetHeight()) / 2;
    dc.Blit(dstX, dstY, bitmap.GetWidth(), bitmap.GetHeight(), &mdc, 0, 0, wxCOPY);

    return result_bitmap.ConvertToImage();
}

wxImage editor::makeSquaredBlur (const wxImage& img, const wxBitmap& bitmap, const int radius) {
    wxImage result = img;
    
    double scale = bitmap.GetWidth() > bitmap.GetHeight()
                    ? (double)bitmap.GetWidth() / bitmap.GetHeight()
                    : (double)bitmap.GetHeight() / bitmap.GetWidth();

    result = result.Scale(bitmap.GetWidth() * scale, bitmap.GetHeight() * scale);
    
    int radii[3];
    double sigma = (2*radius+1) / 6.0;
    boxesForGauss(radii, sigma, 3);

    result = result.Blur(radii[0]);
    result = result.Blur(radii[1]);
    result = result.Blur(radii[2]);

    result = makeSquaredCrop(result);

    wxBitmap result_bitmap(result); // This is slow
    wxMemoryDC dc(result_bitmap);
    
    wxMemoryDC mdc;
    mdc.SelectObjectAsSource(bitmap);

    int dstX = bitmap.GetWidth() > bitmap.GetHeight() ? 0 : (bitmap.GetHeight() - bitmap.GetWidth()) / 2;
    int dstY = bitmap.GetWidth() < bitmap.GetHeight() ? 0 : (bitmap.GetWidth() - bitmap.GetHeight()) / 2;
    dc.Blit(dstX, dstY, bitmap.GetWidth(), bitmap.GetHeight(), &mdc, 0, 0, wxCOPY);

    return result_bitmap.ConvertToImage();
}

//http://blog.ivank.net/fastest-gaussian-blur.html function ported to C++ by me
void editor::boxesForGauss(int v[], int sigma, int n)  // standard deviation, number of boxes
{
    double wIdeal = sqrt((12*sigma*sigma/n)+1);  // Ideal averaging filter width 
    int wl = floor(wIdeal);
    if(wl % 2 == 0) wl--;
    int wu = wl + 2;
				
    double mIdeal = (12*sigma*sigma - n*wl*wl - 4*n*wl - 3*n)/(-4*wl - 4);
    double m = round(mIdeal);
				
    for(int i = 0; i < n; i++) v[i] = i < m ? wl : wu;
}

wxImage editor::compositeImage(const std::vector<wxBitmap>& bitmaps, const wxColour bgColor, int cols, Direction dir) {
    int rows = ceil((double)bitmaps.size() / cols);
    
    int maxWidth = 0;
    int maxHeight = 0;
    for (size_t i = 0; i < bitmaps.size(); i++) {
        if (bitmaps[i].GetWidth() > maxWidth) {
            maxWidth = bitmaps[i].GetWidth();
        }
        if (bitmaps[i].GetHeight() > maxHeight) {
            maxHeight = bitmaps[i].GetHeight();
        }
    }
    wxSize size(maxWidth * cols, maxHeight * rows);
    
    //Good ol' C memory operations
    unsigned char *color = (unsigned char*)malloc(sizeof(unsigned char) * 3);
    color[0] = bgColor.Red();
    color[1] = bgColor.Green();
    color[2] = bgColor.Blue();
    
    unsigned char *rgbData = (unsigned char*)malloc(sizeof(unsigned char) * size.GetWidth() * size.GetHeight() * 3);
    for (int i = 0; i < size.GetWidth()*size.GetHeight(); i++) {
        memcpy(rgbData + sizeof(unsigned char) * 3 * i,
               color,
               sizeof(unsigned char) * 3
        );
    }

    free(color);
    color = NULL;

    wxBitmap result = wxImage(size.GetWidth(), size.GetHeight(), rgbData, false);

    wxMemoryDC dc(result);
    for (size_t i = 0; i < bitmaps.size(); i++) {
        wxBitmap bitmap(bitmaps[i]);
        wxMemoryDC mdc;
        mdc.SelectObjectAsSource(bitmap);

        int dstX, dstY;
        switch (dir) {
            case Direction::LR_TB:
                dstX = (i % cols) * maxWidth + ((maxWidth - bitmaps[i].GetWidth()) / 2);
                dstY = (i / cols) * maxHeight + ((maxHeight - bitmaps[i].GetHeight()) / 2);
                break;
            case Direction::RL_TB:
                dstX = ((bitmaps.size() - i) % cols) * maxWidth + ((maxWidth - bitmaps[i].GetWidth()) / 2);
                dstY = (i / cols) * maxHeight + ((maxHeight - bitmaps[i].GetHeight()) / 2);
                break;
            case Direction::TB_LR:
                dstX = (i / rows) * maxWidth + ((maxWidth - bitmaps[i].GetWidth()) / 2);
                dstY = (i % rows) * maxHeight + ((maxHeight - bitmaps[i].GetHeight()) / 2);
                break;
            case Direction::TB_RL:
                dstX = ((bitmaps.size() - i) / rows) * maxWidth + ((maxWidth - bitmaps[i].GetWidth()) / 2);
                dstY = (i % rows) * maxHeight + ((maxHeight - bitmaps[i].GetHeight()) / 2);
                break;
            default:
                //We shouldn't ever reach this
                dstX = 0;
                dstY = 0;
                break;
        }
        
        dc.Blit(dstX, dstY, bitmap.GetWidth(), bitmap.GetHeight(), &mdc, 0, 0, wxCOPY);
    }

    return result.ConvertToImage();
}