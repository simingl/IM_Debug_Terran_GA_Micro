#include "Dialog.h"
#include "../BWAPI/GameImpl.h"

#include "../Detours.h"
#include "../DLLMain.h"

#include "../../Debug.h"

BYTE gbTinyBtnGfx[3][12*12] = {
  {
    0x00, 0x00, 0x00, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7B, 0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B, 0x7B, 0x00, 0x00,
    0x00, 0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B, 0x00,
    0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B,
    0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B,
    0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B,
    0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B,
    0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B,
    0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B,
    0x00, 0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B, 0x00,
    0x00, 0x00, 0x7B, 0x7B, 0x2B, 0x2B, 0x2B, 0x2B, 0x7B, 0x7B, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xA5, 0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5, 0xA5, 0x00, 0x00,
    0x00, 0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5, 0x00,
    0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5,
    0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5,
    0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5,
    0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5,
    0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5,
    0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5,
    0x00, 0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5, 0x00,
    0x00, 0x00, 0xA5, 0xA5, 0x2B, 0x2B, 0x2B, 0x2B, 0xA5, 0xA5, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xC7, 0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7, 0xC7, 0x00, 0x00,
    0x00, 0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7, 0x00,
    0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7,
    0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7,
    0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7,
    0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7,
    0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7,
    0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7,
    0x00, 0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7, 0x00,
    0x00, 0x00, 0xC7, 0xC7, 0x2B, 0x2B, 0x2B, 0x2B, 0xC7, 0xC7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0x00, 0x00, 0x00
  }
};

BYTE bFontColors[24][8] = {
  { 0xC0, 0x9B, 0x9A, 0x95, 0x43, 0x00, 0x00, 0x28 }, 
  { 0x56, 0xA7, 0x6D, 0x65, 0x5C, 0x00, 0x00, 0x8A }, 
  { 0x41, 0xFF, 0x53, 0x97, 0x47, 0x00, 0x00, 0x8A }, 
  { 0x40, 0x96, 0x49, 0x90, 0x42, 0x00, 0x00, 0x8A }, 
  { 0xA8, 0xAE, 0x17, 0x5E, 0xAA, 0x00, 0x00, 0x8A }, 
  { 0xB5, 0x75, 0xBA, 0xB9, 0xB7, 0x00, 0x00, 0x8A }, 
  { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 }, 
  { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 }, 
  { 0x8A, 0x6F, 0x17, 0x5E, 0xAA, 0x8A, 0x8A, 0x8A }, 
  { 0x28, 0xA5, 0xA2, 0x2D, 0xA0, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x9F, 0x9E, 0x9D, 0xB7, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0xA4, 0xA3, 0xA1, 0x0E, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x9C, 0x1C, 0x1A, 0x13, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x13, 0x12, 0x11, 0x57, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x54, 0x51, 0x4E, 0x4A, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x87, 0xA6, 0x81, 0x93, 0x8A, 0x8A, 0x8A }, 
  { 0xB5, 0xB9, 0xB8, 0xB7, 0xB6, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x88, 0x84, 0x81, 0x60, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x86, 0x72, 0x70, 0x69, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x33, 0x7C, 0x7A, 0xA0, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x4D, 0x26, 0x23, 0x22, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x9A, 0x97, 0x95, 0x91, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x88, 0x84, 0x81, 0x60, 0x8A, 0x8A, 0x8A }, 
  { 0x8A, 0x80, 0x34, 0x31, 0x2E, 0x8A, 0x8A, 0x8A }
};

BYTE bColorTable[] = { 
   0,  0,  0,  1,  2,  3,  4,  5,  8,  0,  0,  0,  0,  0,  9, 10, 
  11, 12,  0,  0,  0, 13, 14, 15, 16, 17,  0, 18, 19, 20, 21, 23
};

namespace BW
{
  // ------------------ TEMPLATE ---------------------
  bool __fastcall InteractTemplate(dialog *dlg, dlgEvent *evt)
  {
    char out[512];
    switch (evt->wNo)
    {
    case 0: // Key down
      break;
    case 1: // Key repeat
      break;
    case 2: // Key Modifier?? Key up?
      break;
    case 3: // Mouse Move
      break;
    case 4: // Left Button down
      break;
    case 5: // Left Button up
      break;
    case 6: // Left Button double click
      break;
    case 7: // Right Button Down
      break;
    case 8: // Right Button Up
      break;
    case 9: // Right Button double click
      break;
    case 10: // Middle Button Down
      break;
    case 11: // Middle Button Up
      break;
    case 12: // Middle Button double click
      break;
    case 13: // Idle
      break;
    case 14: // Control (used for when a control has been pressed)
      switch(evt->dwUser)
      {
      case 0: // Update/Create
        break;
      case 1: // Kill
        break;
      case 2: // Activate
        break;
      case 3: // Press Hotkey
        break;
      case 4: // MouseOver
        break;
      case 5: // Select Previous
        break;
      case 6: // Select Next
        break;
      case 7: // Initialize dialog? Loop? Update?
        break;
      case 8: // Unk Init control
        break;
      case 9: // MouseEnter
        break;
      case 10: // Initialize children
        break;
      case 11: // Set Selected Index
        break;
      case 12:  //      unknown
        break;
      case 13: // Show
        break;
      case 14: // Hide
        break;
      case 15: // Scroll up
        break;
      case 16: // Scroll down
        break;
      default:
        sprintf_s(out, 512, "Event: %d\nUser: 0x%p", evt->wNo, evt->dwUser);
        MessageBoxA(NULL, out, "!", MB_OK);
        break;
      }
      break;
    case 15: // User Key Press (like WM_CHAR)
      break;
    case 16: // System Key Press
      break;
    case 17: // Mouse wheel Scroll up
      break;
    case 18: // Mouse wheel Scroll down
      break;
    default:
      sprintf_s(out, 512, "Unknown Event: %d\nUser: 0x%p", evt->wNo, evt->dwUser);
      MessageBoxA(NULL, out, "!", MB_OK);
      break;
    }
    return dlg->defaultInteract(evt);
  }
  // ------------------ BUTTON INTERACT --------------
  bool __fastcall TinyButtonInteract(dialog *dlg, dlgEvent *evt)
  {
    switch (evt->wNo)
    {
    case 2:
    case 5: // Left Button up
      dlg->srcBits.data = gbTinyBtnGfx[0];
      break;
    case 4:
    case 6: // Left Button down
      dlg->srcBits.data = gbTinyBtnGfx[2];
      break;
    case 14: // Control (used for when a control has been pressed)
      switch(evt->dwUser)
      {
      case 2: // Activate
        switch (dlg->wIndex)
        {
        case 255: // minimize
          if ( (WORD)dlg->lUser )
          {
            dlg->parent()->srcBits.ht = (WORD)dlg->lUser;
            dlg->parent()->rct.bottom = dlg->parent()->rct.top + dlg->parent()->srcBits.ht - 1;
            dlg->parent()->u.dlg.dstBits.ht = dlg->parent()->srcBits.ht;
            dlg->lUser = 0;
          }
          else
          {
            dlg->lUser = dlg->parent()->srcBits.ht;
            dlg->parent()->srcBits.ht = 13;
            dlg->parent()->rct.bottom = dlg->parent()->rct.top + dlg->parent()->srcBits.ht - 1;
            dlg->parent()->u.dlg.dstBits.ht = dlg->parent()->srcBits.ht;
          }
          wantRefresh = true;
          break;
        }
        break;
      case 4: // mouseOver
        dlg->srcBits.data = gbTinyBtnGfx[1];
        break;
      case 3: // hotkey
      case 5:
      case 6: // Select Previous/Next
        return false;
      }
      break;
    }
    return dlg->defaultInteract(evt);
  }
  // ------------------ WINDOW INTERACT --------------
  bool __fastcall WindowInteract(dialog *dlg, dlgEvent *evt)
  {
    pt *mouseOffset = (pt*)&dlg->lUser;
    dialog *i;
    switch (evt->wNo)
    {
    case 3: // Mouse update/Move
      if ( dlg->wUnk_0x1E )
      {
        dlg->rct.left   = evt->cursor.x - mouseOffset->x;
        dlg->rct.right  = dlg->rct.left + dlg->srcBits.wid - 1;
        dlg->rct.top    = evt->cursor.y - mouseOffset->y;
        dlg->rct.bottom = dlg->rct.top + dlg->srcBits.ht - 1;
        rect scrLimit   = { 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };
        if ( dlg->rct.left < 0 )
        {
          dlg->rct.right -= dlg->rct.left;
          dlg->rct.left  -= dlg->rct.left;
        }
        if ( dlg->rct.top < 0 )
        {
          dlg->rct.bottom -= dlg->rct.top;
          dlg->rct.top    -= dlg->rct.top;
        }
        if ( dlg->rct.right > scrLimit.right )
        {
          dlg->rct.left  -= dlg->rct.right - scrLimit.right;
          dlg->rct.right -= dlg->rct.right - scrLimit.right;
        }
        if ( dlg->rct.bottom > (scrLimit.bottom - 40) )
        {
          dlg->rct.top    -= dlg->rct.bottom - (scrLimit.bottom - 40);
          dlg->rct.bottom -= dlg->rct.bottom - (scrLimit.bottom - 40);
        }
        wantRefresh = true;
      }
      i = dlg->child();
      while( i )
      {
        if ( !(evt->cursor.x >= (dlg->rct.left + i->rct.left) &&
             evt->cursor.x <= (dlg->rct.left + i->rct.right) &&
             evt->cursor.y >= (dlg->rct.top + i->rct.top) &&
             evt->cursor.y <= (dlg->rct.top + i->rct.bottom))
           )
        {
          switch( i->wIndex )
          {
            case 255: // minimize
            case -2: // close
              i->srcBits.data = gbTinyBtnGfx[0];
              break;
          }
        }
        i = i->next();
      }
      break;
    case 4: // Left Button down
      if ( evt->cursor.x >= dlg->rct.left &&
         evt->cursor.x <= dlg->rct.right - 27 &&
         evt->cursor.y >= dlg->rct.top &&
         evt->cursor.y <= dlg->rct.top + 12 )
      {
        mouseOffset->x = evt->cursor.x - dlg->rct.left;
        mouseOffset->y = evt->cursor.y - dlg->rct.top;
        dlg->wUnk_0x1E = 1;
      }
      break;
    case 5: // Left Button up
      dlg->wUnk_0x1E = 0;
      break;
    case 14: // user
      switch ( evt->dwUser )
      {
      case 2:
        {
        bool rval = dlg->defaultInteract(evt);
        //delete dlg;
        //if ( strcmpi(dlg->getText(), "Test Dialog") == 0)
        BWAPI::BroodwarImpl.myDlg = NULL;
        return rval;
        }
      }
      break;
    }
    return dlg->defaultInteract(evt);
  }
  // ------------------ CREATE DLG WINDOW ------------
  dialog *CreateDialogWindow(const char *pszText, WORD wLeft, WORD wTop, WORD wWidth, WORD wHeight)
  {
    // Create dialog
    dialog *dlg = new dialog(ctrls::cDLG, 0, pszText, wLeft, wTop, wWidth, wHeight, &WindowInteract);
    dlg->applyWindowBackground();

    // Create title text
    dialog *title = new dialog(ctrls::cLSTATIC, -255, pszText, 8, 1, wWidth - 27, 12);
    title->setFlags(CTRL_FONT_SMALLEST);
    dlg->addControl(title);

    // Create minimize button
    dialog *minimize = new dialog(ctrls::cBUTTON, 255, " _", wWidth - 26, 1, 12, 12, &TinyButtonInteract);
    minimize->setFlags(CTRL_FONT_SMALLEST);
    minimize->srcBits.data = gbTinyBtnGfx[0];
    dlg->addControl(minimize);

    // Create close button
    dialog *close = new dialog(ctrls::cBUTTON, -2, " X", wWidth - 13, 1, 12, 12, &TinyButtonInteract);
    close->setFlags(CTRL_FONT_SMALLEST);
    close->srcBits.data = gbTinyBtnGfx[0];
    dlg->addControl(close);

    return dlg;
  }
  // ------------------ FIND GLOBAL ------------------
  dialog *FindDialogGlobal(const char *pszName)
  {
    if ( (*BW::BWDATA_DialogList) && pszName )
      return (*BW::BWDATA_DialogList)->findDialog(pszName);
    return NULL;
  }
  // ----------------- GET WIDTH ---------------------
  int GetTextWidth(const char *pszString, BYTE bSize)
  {
    // verify valid size index
    if ( bSize > 3 || !pszString )
      return 0;

    // localize pointer
    fntHead *font = BWDATA_FontBase[bSize];
    if ( !font )
      return 0;

    // Reference an unsigned character array
    const BYTE *pbChars = (BYTE*)pszString;

    // Retrieve size
    int dwSize = 0;
    for ( int i = 0; pbChars[i]; ++i )
    {
      switch ( pbChars[i] )
      {
      case 9:
        dwSize += font->Xmax * 2;
        continue;
      case ' ':
        dwSize += font->Xmax >> 1;
        continue;
      }

      // must be valid character
      if ( pbChars[i] > font->high || pbChars[i] < font->low)
        continue;

      // localize character pointer
      fntChr *chr = font->chrs[pbChars[i] - font->low];
      if ( chr == (fntChr*)font )
        continue;

      // increase the size
      dwSize += chr->w;
    }
    return dwSize;
  }
  // ----------------- GET WIDTH ---------------------
  int GetTextHeight(const char *pszString, BYTE bSize)
  {
    // verify valid size index
    if ( bSize > 3 || !pszString )
      return 0;

    // localize pointer
    fntHead *font = BWDATA_FontBase[bSize];
    if ( !font )
      return 0;

    // Reference an unsigned character array
    const BYTE *pbChars = (BYTE*)pszString;

    // Retrieve size
    int dwSize = font->Ymax;
    for ( int i = 0; pbChars[i]; ++i )
    {
      switch ( pbChars[i] )
      {
      case 10:
        dwSize += font->Ymax;
        break;
      }
    }
    return dwSize;
  }
  // ----------------- BLIT TEXT ---------------------
  bool BlitText(const char *pszString, bitmap *dst, int x, int y, BYTE bSize)
  {
    // verify valid size index
    if ( bSize > 3 || !dst || !pszString )
      return false;

    // localize pointer
    fntHead *font = BWDATA_FontBase[bSize];
    if ( !font || !dst->data )
      return false;

    // Reference an unsigned character array
    const BYTE *pbChars = (BYTE*)pszString;

    BYTE lastColor = 0, color   = 0;
    int  Xoffset   = x, Yoffset = y;

    // Iterate all characters in the message
    for ( int c = 0; pbChars[c]; ++c )
    {
      // Perform control character and whitespace functions
      if ( pbChars[c] <= ' ' )
      {
        switch ( pbChars[c] )
        {
        case 1:
          color = lastColor;
          continue;
        case 9:
          Xoffset += font->Xmax * 2;
          continue;
        case 10:
          Xoffset = x;
          Yoffset += font->Ymax;
          continue;
        case 11:
        case 20:
          color = (BYTE)~0;
          continue;
        case 12:
          break;
        case 13:
        case 26:
          continue;
        case 18:
          Xoffset += dst->wid - GetTextWidth(pszString, bSize) - x;
          continue;
        case 19:
          Xoffset += dst->wid / 2 - GetTextWidth(pszString, bSize) / 2 - x;
          continue;
        case ' ':
          Xoffset += font->Xmax / 2;
          continue;
        default:
          lastColor = color;
          color     = bColorTable[pbChars[c]];
          continue;
        }
      }

      // Skip if the character is not supported by the font
      if ( pbChars[c] > font->high || pbChars[c] < font->low )
        continue;

      // localize character pointer
      fntChr *chr = font->chrs[pbChars[c] - font->low];
      if ( chr == (fntChr*)font )
        continue;

      if ( color != ~0 )
      {
        // begin drawing character process
        for ( int i = 0, pos = 0; pos < chr->h * chr->w; ++i, ++pos )
        {
          // font position
          pos += chr->data[i] >> 3;

          // x offset
          int newX = Xoffset + (chr->x + pos % chr->w);
          if ( newX >= dst->wid ) break;
          if ( newX < 0 ) continue;

          // y offset
          int newY = Yoffset + (chr->y + pos / chr->w);
          if ( newY >= dst->ht ) break;
          if ( newY < 0 ) continue;

          // blit offset
          int offset = newY * dst->wid + newX;
          if ( offset >= dst->wid * dst->ht ) break;
          if ( offset < 0 ) continue;

          // Plot pixel
          dst->data[offset] = bFontColors[color][chr->data[i] & 0x07];
        }
      }
      Xoffset += chr->w;
    }
    return true;
  }
  // ----------------- CONSTRUCTORS ------------------
  dialog::dialog(WORD ctrlType, short index, const char *text, WORD left, WORD top, WORD width, WORD height, bool (__fastcall *pfInteract)(dialog*,dlgEvent*))
    : pNext( NULL )
    , pszText( (char*)text )
    , lFlags( CTRL_VISIBLE )
    , wIndex( index )
    , wCtrlType( ctrlType )
  {
    if ( ctrlType > ctrls::max)
      ctrlType = ctrls::cLSTATIC;

    // Set default height
    if ( height == 0 )
    {
      switch ( ctrlType )
      {
      case ctrls::cBUTTON:
      case ctrls::cDFLTBTN:
        height = 28;
        break;
      case ctrls::cCHKBOX:
      case ctrls::cOPTION:
        height = 20;
        break;
      case ctrls::cEDIT:
        height = 16;
        break;
      }
    }

    // Set size properties
    rct.left    = left;
    rct.top     = top;
    rct.right   = rct.left + width - 1;
    rct.bottom  = rct.top  + height - 1;
    srcBits.wid = width;
    srcBits.ht  = height;

    // Set callback functions
    pfcnUpdate  = BW::BWDATA_GenericDlgUpdateFxns[wCtrlType];
    if ( pfInteract )
      pfcnInteract = pfInteract;
    else
      pfcnInteract = BW::BWDATA_GenericDlgInteractFxns[wCtrlType];

    // Set control type-specific options
    switch ( ctrlType )
    {
    case ctrls::cDLG:
      lFlags          |= CTRL_DLG_NOREDRAW | CTRL_TRANSLUCENT;
      this->applyDialogBackground();

      // Allocate destination buffer
      u.dlg.dstBits.wid   = width;
      u.dlg.dstBits.ht    = height;
      u.dlg.dstBits.data  = (BYTE*)SMAlloc(width*height);
      break;
    case ctrls::cBUTTON:
    case ctrls::cDFLTBTN:
    case ctrls::cFLCBTN:
      lFlags   |= CTRL_USELOCALGRAPHIC | CTRL_FONT_LARGE | CTRL_TRANSLUCENT;
      wGraphic = 112;
    case ctrls::cCHKBOX:
    case ctrls::cOPTION:
    case ctrls::cHSCROLL:
    case ctrls::cVSCROLL:
      lFlags |= CTRL_EVENTS;
      break;
    case ctrls::cLIST:
    case ctrls::cCOMBO:
      lFlags |= CTRL_TRANSPARENT;
    case ctrls::cEDIT:
      lFlags |= CTRL_EVENTS | CTRL_BTN_NO_SOUND;
      break;
    case ctrls::cIMAGE:
      this->applyDialogBackground();
      break;
    }
  }
  // ------------------ DESTRUCTOR -------------------
  dialog::~dialog()
  {
    if ( this )
    {
      if ( this->isDialog() )
      {
        if ( this->isListed() )
          this->activate();

        if ( this->u.dlg.dstBits.data )
          SMFree(this->u.dlg.dstBits.data);
      }
      else
      {
        if ( this->isList() )
        {
          if ( this->u.list.pbStrFlags )
            SMFree(this->u.list.pbStrFlags);
          if ( this->u.list.pdwData )
            SMFree(this->u.list.pdwData);
          if ( this->u.list.ppStrs )
            SMFree(this->u.list.ppStrs);
        }

        if ( this->wCtrlType == ctrls::cEDIT && this->pszText )
          SMFree(this->pszText);

        if ( this->parent() )
        {
          if ( this->parent()->u.dlg.pActiveElement == this )
            this->parent()->u.dlg.pActiveElement = NULL;

          if ( this->parent()->u.dlg.pMouseElement == this )
            this->parent()->u.dlg.pMouseElement = NULL;

          if ( this->parent()->u.dlg.pFirstChild == this )
          {
            this->parent()->u.dlg.pFirstChild = this->next();
            return;
          }

          dialog *i = this->parent()->child();
          do
          {
            if ( i->pNext == this )
            {
              i->pNext = this->next();
              return;
            }
            i = i->next();
          } while (i);
        } // if parent
      } // if control
    } // if this
  }
  // --------------------- FIND ----------------------
  dialog *dialog::findIndex(short wIndex)
  {
    if ( !this )
      return NULL;

    dialog *pCurrDlg;
    if ( this->isDialog() )
      pCurrDlg = this;
    else
      pCurrDlg = this->parent();

    if ( pCurrDlg )
      pCurrDlg = pCurrDlg->child();

    while ( pCurrDlg && pCurrDlg->getIndex() != wIndex )
      pCurrDlg = pCurrDlg->next();
    
    if ( pCurrDlg->getIndex() == wIndex )
      return pCurrDlg;

    return NULL;
  }
  // ------------------ FIND BY NAME -----------------
  dialog *dialog::findDialog(const char *pszName)
  {
    if ( this )
    {
      dialog *parent = this;
      if ( !parent->isDialog() )
        parent = parent->parent();
      
      while ( parent )
      {
        if (parent->pszText && strcmpi(parent->pszText, pszName) == 0 )
          return parent;
        parent = parent->next();
      }
    }
    return NULL;
  }
  // --------------------- NEXT ----------------------
  dialog *dialog::next()
  {
    if ( this && this->pNext )
      return this->pNext;
    return NULL;
  }
  // ------------------- SET FLAG --------------------
  bool dialog::setFlags(DWORD dwFlag)
  {
    if ( this )
    {
      if ( !this->isDialog() )
      {
        DWORD size    = dwFlag & (CTRL_FONT_SMALLEST | CTRL_FONT_SMALL | CTRL_FONT_LARGE | CTRL_FONT_LARGEST);
        DWORD halign  = dwFlag & (CTRL_HALIGN_LEFT | CTRL_HALIGN_RIGHT | CTRL_HALIGN_CENTER);
        DWORD valign  = dwFlag & (CTRL_VALIGN_TOP | CTRL_VALIGN_MIDDLE | CTRL_VALIGN_BOTTOM);
        DWORD remains = dwFlag & ~(size | halign | valign);

        // set size
        if (size == CTRL_FONT_SMALLEST ||
            size == CTRL_FONT_SMALL ||
            size == CTRL_FONT_LARGE ||
            size == CTRL_FONT_LARGEST)
        {
          this->clearFlags(CTRL_FONT_SMALLEST | CTRL_FONT_SMALL | CTRL_FONT_LARGE | CTRL_FONT_LARGEST);
          this->lFlags |= size;
        }

        // set horizontal alignment
        if (halign == CTRL_HALIGN_LEFT ||
            halign == CTRL_HALIGN_RIGHT ||
            halign == CTRL_HALIGN_CENTER)
        {
          this->clearFlags(CTRL_HALIGN_LEFT | CTRL_HALIGN_RIGHT | CTRL_HALIGN_CENTER);
          this->lFlags |= halign;
        }

        // set vertical alignment
        if (valign == CTRL_VALIGN_TOP ||
            valign == CTRL_VALIGN_MIDDLE ||
            valign == CTRL_VALIGN_BOTTOM)
        {
          this->clearFlags(CTRL_VALIGN_TOP | CTRL_VALIGN_MIDDLE | CTRL_VALIGN_BOTTOM);
          this->lFlags |= valign;
        }

        this->lFlags |= remains;
      }
      else
      {
        this->lFlags |= dwFlag;
      }
      return true;
    }
    return false;
  }
  // ------------------ CLEAR FLAG -------------------
  bool dialog::clearFlags(DWORD dwFlag)
  {
    if ( this )
    {
      this->lFlags &= ~dwFlag;
      return true;
    }
    return false;
  }
  // ------------------- HAS FLAG --------------------
  bool dialog::hasFlags(DWORD dwFlag)
  {
    if ( this && (this->lFlags & dwFlag) == dwFlag)
      return true;
    return false;
  }
  // ------------------- SET TEXT --------------------
  bool dialog::setText(char *pszStr)
  {
    if ( this && pszStr )
    {
      if ( this->wCtrlType == ctrls::cEDIT && this->pszText)
        strcpy_s(this->pszText, 255, pszStr);
      else
        this->pszText = pszStr;
      return true;
    }
    return false;
  }
  // ------------------- GET TEXT --------------------
  char *dialog::getText()
  {
    if ( this && this->pszText )
      return this->pszText;
    return "";
  }
  // ------------------ GET HOTKEY -------------------
  int dialog::getHotkey()
  {
    if ( this && this->pszText && this->isButton() )
      return toupper(this->pszText[0]);
    return 0;
  }
  // ------------------ SRC BUFFER -------------------
  BW::bitmap *dialog::getSourceBuffer()
  {
    if ( this )
      return &this->srcBits;
    return NULL;
  }
// -------------------- ENABLE -----------------------
  bool dialog::enable()
  {
    if ( this )
    {
      this->clearFlags(CTRL_DISABLED);
      return true;
    }
    return false;
  }
// -------------------- DISABLE -----------------------
  bool dialog::disable()
  {
    if ( this )
    {
      this->setFlags(CTRL_DISABLED);
      return true;
    }
    return false;
  }
// ------------------ IS DISABLED --------------------
  bool dialog::isDisabled()
  {
    if ( this && this->hasFlags(CTRL_DISABLED) )
      return true;
    return false;
  }
// --------------------- SHOW ------------------------
  bool dialog::show()
  {
    if ( this )
    {
      this->doEvent(14, 13);
      this->setFlags(CTRL_VISIBLE);
      return true;
    }
    return false;
  }
// --------------------- HIDE ------------------------
  bool dialog::hide()
  {
    if ( this )
    {
      this->doEvent(14, 14);
      this->clearFlags(CTRL_VISIBLE);
      return true;
    }
    return false;
  }
// ------------------ IS VISIBLE ---------------------
  bool dialog::isVisible()
  {
    if ( this && this->hasFlags(CTRL_VISIBLE) )
      return true;
    return false;
  }
// ------------------- GET SIZE ----------------------
  u16 dialog::width()
  {
    if ( this )
      return this->srcBits.wid;
    return 0;
  }
  u16 dialog::height()
  {
    if ( this )
      return this->srcBits.ht;
    return 0;
  }
// -------------------------------------------------- EVENTS -------------------------------------------------
  // --------------------- EVENT ---------------------
  bool dialog::doEvent(WORD wEvtNum, DWORD dwUser, WORD wSelect, WORD wVirtKey)
  {
    if ( this && this->pfcnInteract )
    {
      dlgEvent evt;
      evt.cursor.x    = (WORD)BW::BWDATA_Mouse->x;
      evt.cursor.y    = (WORD)BW::BWDATA_Mouse->y;
      evt.wVirtKey    = wVirtKey;
      evt.wSelection  = wSelect;
      evt.wNo         = wEvtNum;
      evt.dwUser      = dwUser;
      return this->pfcnInteract(this, &evt);
    }
    return false;
  }
  // ----------------- DEFAULT INTERACT --------------
  bool dialog::defaultInteract(BW::dlgEvent *pEvent)
  {
    if ( this && pEvent && this->wCtrlType < ctrls::max )
      return BWDATA_GenericDlgInteractFxns[this->wCtrlType](this, pEvent);
    return false;
  }
  // -------------------- ACTIVATE -------------------
  bool dialog::activate()
  {
    if ( this )
      return this->doEvent(14, 2);
    return false;
  }
  // --------------------- UPDATE --------------------
  bool dialog::update()
  {
    if ( this )
      return this->doEvent(14, 0);
    return false;
  }
// -------------------------------------------------- DIALOG -------------------------------------------------
  // --------------------- IS DLG --------------------
  bool dialog::isDialog()
  {
    if ( this && this->wCtrlType == ctrls::cDLG )
      return true;
    return false;
  }
  // --------------------- CHILD ---------------------
  dialog *dialog::child()
  {
    if (this && this->isDialog() && this->u.dlg.pFirstChild )
      return this->u.dlg.pFirstChild;
    return NULL;
  }
  // ------------------ DST BUFFER -------------------
  BW::bitmap *dialog::getDestBuffer()
  {
    if ( this && this->isDialog() )
      return &this->u.dlg.dstBits;
    return NULL;
  }
  // --------------------- ADD -----------------------
  bool dialog::addControl(dialog *ctrl)
  {
    if ( !this || !ctrl || ctrl->isDialog())
      return false;

    dialog *parent = this;
    if ( !parent->isDialog() )
      parent = parent->parent();

    ctrl->u.ctrl.pDlg = parent;
    if ( !parent->child() )
    {
      parent->u.dlg.pFirstChild = ctrl;
    }
    else
    {
      dialog *child = parent->child();
      while ( child->pNext )
        child = child->pNext;
      child->pNext = ctrl;
    }
    return true;
  }
  // ------------------ INITIALIZE -------------------
  bool dialog::initialize()
  {
    if ( this && this->isDialog() )
    {
      this->doEvent(14, 7);
      this->doEvent(14, 10);
      this->update();
      return true;
    }
    return false;
  }
  // ------------------ IS LISTED --------------------
  bool dialog::isListed()
  {
    if ( this )
    {
      for ( dialog *i = *BW::BWDATA_DialogList; i; i = i->next() )
      {
        if ( this == i )
          return true;
      }
    }
    return false;
  }
  // --------- APPLY DEFAULT BACKGROUND --------------
  bool dialog::applyDialogBackground()
  {
    if ( this )
    {
      if ( this->applyBlankBackground() && this->srcBits.data )
      {
        // localize the variables
        BYTE *data  = this->srcBits.data;
        WORD width  = this->srcBits.wid;
        WORD height = this->srcBits.ht;

        // first line
        memset(&data[3], 0x2A, width - 6);

        // second line
        memset(&data[width + 1], 0x2A, 2);
        memset(&data[width + 3], 0x29, width - 6);
        memset(&data[width*2 - 3], 0x2A, 2);

        // third line
        data[width*2 + 1] = 0x2A;
        memset(&data[width*2 + 2], 0x29, width - 4);
        data[width*3 - 2] = 0x2A;

        // body
        for ( int i = 3; i < height - 3; ++i )
        {
          data[width*i] = 0x2A;
          memset(&data[width*i + 1], 0x29, width - 2);
          data[width*(i+1) - 1] = 0x2A;
        }

        // third-last line
        data[width*height - width*3 + 1] = 0x2A;
        memset(&data[width*height - width*3 + 2], 0x29, width - 4);
        data[width*height - width*2 - 2] = 0x2A;

        // second-last line
        memset(&data[width*height - width*2 + 1], 0x2A, 2);
        memset(&data[width*height - width*2 + 3], 0x29, width - 6);
        memset(&data[width*height - width - 3], 0x2A, 2);

        // last line
        memset(&data[width*height - width + 3], 0x2A, width - 6);
        return true;
      }
    }
    return false;
  }
  // --------- APPLY WINDOW BACKGROUND ---------------
  bool dialog::applyWindowBackground()
  {
    if ( this )
    {
      if ( this->applyDialogBackground() && this->srcBits.data )
      {
        // localize the variables
        BYTE *data  = this->srcBits.data;
        WORD width  = this->srcBits.wid;

        // Create title bar
        memset(&data[width + 3], 0x2C, width - 6);
        memset(&data[width*2 + 2], 0x2C, width - 4);
        for ( int i = 3; i < 12; ++i )
          memset(&data[width*i + 1], 0x2C, width - 2);
        return true;
      }
    }
    return false;
  }
  // ----------- APPLY BLANK BACKGROUND --------------
  bool dialog::applyBlankBackground()
  {
    if ( this )
    {
      // localize the size
      WORD width  = srcBits.wid;
      WORD height = srcBits.ht;
      
      // create the buffer if not already
      if ( !this->srcBits.data )
        srcBits.data    = (BYTE*)SMAlloc(width*height);

      // localize the buffer pointer
      BYTE *data  = srcBits.data;
      if ( data )
      {
        // set the data
        memset(data, 0, width * height);
        return true;
      }
    }
    return false;
  }
// -------------------------------------------------- CONTROL ------------------------------------------------
  // -------------------- PARENT ---------------------
  dialog *dialog::parent()
  {
    if ( this && !this->isDialog() && this->u.ctrl.pDlg )
      return this->u.ctrl.pDlg;
    return NULL;
  }
  // --------------------- INDEX ---------------------
  short dialog::getIndex()
  {
    if ( this )
      return this->wIndex;
    return 0;
  }
  // ------------- CLEAR FONT FLAGS ------------------
  bool dialog::clearFontFlags()
  {
    if ( this )
    {
      this->clearFlags(CTRL_FONT_SMALLEST | CTRL_FONT_SMALL | CTRL_FONT_LARGE | CTRL_FONT_LARGEST);
      this->clearFlags(CTRL_HALIGN_LEFT | CTRL_HALIGN_RIGHT | CTRL_HALIGN_CENTER);
      this->clearFlags(CTRL_VALIGN_TOP | CTRL_VALIGN_MIDDLE | CTRL_VALIGN_BOTTOM);
      return true;
    }
    return false;
  }
// -------------------------------------------- BUTTON -------------------------------------------------------
  // ------------------- IS BUTTON -------------------
  bool dialog::isButton()
  {
    if ( this )
    {
      WORD wCtrl = this->wCtrlType;
      if ( wCtrl == ctrls::cBUTTON || wCtrl == ctrls::cDFLTBTN || wCtrl == ctrls::cFLCBTN )
        return true;
    }
    return false;
  }
// -------------------------------------------- CHECKBOX & OPTION --------------------------------------------
  // ------------------- IS CHECKBOX -----------------
  bool dialog::isOption()
  {
    if ( this )
    {
      WORD wCtrl = this->wCtrlType;
      if ( wCtrl == ctrls::cCHKBOX || wCtrl == ctrls::cOPTION)
        return true;
    }
    return false;
  }
  // --------------------- CHECKED -------------------
  bool dialog::isChecked()
  {
    if ( this && this->isOption() )
      return this->u.optn.bEnabled != 0;
    return false;
  }
// -------------------------------------------- LISTBOX & COMBOBOX -------------------------------------------
  // ----------------------- IS LIST -----------------
  bool dialog::isList()
  {
    if ( this )
    {
      WORD wCtrl = this->wCtrlType;
      if ( wCtrl == ctrls::cCOMBO || wCtrl == ctrls::cLIST )
        return true;
    }
    return false;
  }
  // --------------- GET SELECTED INDEX --------------
  BYTE dialog::getSelectedIndex()
  {
    if ( this && this->isList() )
      return this->u.list.bSelectedIndex;
    return 0;
  }
  // --------------- GET SELECTED VALUE --------------
  DWORD dialog::getSelectedValue()
  {
    if ( this 
        && this->isList() 
        && this->u.list.pdwData 
        && this->u.list.bSelectedIndex < this->u.list.bStrs)
      return this->u.list.pdwData[this->u.list.bSelectedIndex];
    return 0;
  }
  // --------------- GET SELECTED STRING -------------
  char *dialog::getSelectedString()
  {
    if ( this && this->isList() && this->u.list.ppStrs && this->u.list.ppStrs[this->u.list.bCurrStr])
      return this->u.list.ppStrs[this->u.list.bCurrStr];
    return "";
  }
// -------------- SET SELECTED INDEX -----------------
  bool dialog::setSelectedIndex(BYTE bIndex)
  {
    if ( this && this->isList() && bIndex < this->u.list.bStrs )
    {
      this->doEvent(14, 11, bIndex);
      if ( this->u.list.pScrlBar )
        this->u.list.pScrlBar->doEvent(14, 11, bIndex);
      return true;
    }
    return false;
  }
// -------------- SET SELECTED BY VALUE --------------
  bool dialog::setSelectedByValue(DWORD dwValue)
  {
    if ( this && this->isList() && this->u.list.pdwData )
    {
      for ( int i = 0; i < this->u.list.bStrs; ++i )
      {
        if ( this->u.list.pdwData[i] == dwValue )
        {
          this->doEvent(14, 11, (WORD)i);
          if ( this->u.list.pScrlBar )
            this->u.list.pScrlBar->doEvent(14, 11, (WORD)i);
          return true;
        }
      }
    } // check
    return false;
  }
// -------------- SET SELECTED BY STRING -------------
  bool dialog::setSelectedByString(const char *pszString)
  {
    // verify that this is the correct control
    if ( this && this->isList() && this->u.list.ppStrs )
    {
      // Iterate through each entry
      for ( int i = 0; i < this->u.list.bStrs; ++i )
      {
        // compare the string to the one we're looking for
        if ( this->u.list.ppStrs[i] && strcmpi(this->u.list.ppStrs[i], pszString) == 0 )
        {
          // set the selected entry
          this->doEvent(14, 11, (WORD)i);
          if ( this->u.list.pScrlBar )
            this->u.list.pScrlBar->doEvent(14, 11, (WORD)i);
          return true;
        }
      } // iterator
    }
    return false;
  }
  // ------------------- ADD ENTRY -------------------
  bool dialog::addListEntry(char *pszString, DWORD dwValue, BYTE bFlags)
  {
    if ( this && this->isList() && this->u.list.pbStrFlags && this->u.list.pdwData && this->u.list.ppStrs )
    {
      BYTE count = this->u.list.bStrs;
      if ( count < 255 )
      {
        this->u.list.pbStrFlags[count] = bFlags;
        this->u.list.pdwData[count]    = dwValue;
        this->u.list.ppStrs[count]     = pszString;
        this->u.list.bStrs++;
        dialog *scroll = this->u.list.pScrlBar;
        if ( scroll && this->u.list.bStrs > this->u.list.bItemsPerPage )
        {
          scroll->u.scroll.nMax++;
          scroll->show();
          scroll->update();
        }
        return true;
      }
    }
    return false;
  }
  // ----------------- REMOVE ENTRY ------------------
  bool dialog::removeListEntry(BYTE bIndex)
  {
    if ( this && this->isList() && bIndex < this->u.list.bStrs )
    {
      for ( int i = bIndex; i < this->u.list.bStrs; ++i )
      {
        dialog *scroll = this->u.list.pScrlBar;
        if ( scroll && scroll->u.scroll.nMax > scroll->u.scroll.nMin )
          this->u.list.pScrlBar->u.scroll.nMax--;

        if ( i == this->u.list.bStrs - 1 )
        {
          this->u.list.pbStrFlags[i]  = 0;
          this->u.list.pdwData[i]     = 0;
          this->u.list.ppStrs[i]      = NULL;
          this->u.list.bStrs--;
          if ( this->u.list.bSelectedIndex >= bIndex )
            this->u.list.bSelectedIndex--;
          if ( this->u.list.bCurrStr >= bIndex )
            this->u.list.bCurrStr--;
          if ( this->u.list.bOffset + this->u.list.bItemsPerPage > this->u.list.bStrs && this->u.list.bStrs > this->u.list.bItemsPerPage - 1 )
            this->u.list.bOffset--;

          if ( scroll && this->u.list.bStrs <= this->u.list.bItemsPerPage )
          {
            scroll->hide();
            scroll->update();
          }

          this->doEvent(14, 11, this->getSelectedIndex());
        }
        else
        {
          this->u.list.pbStrFlags[i]  = this->u.list.pbStrFlags[i+1];
          this->u.list.pdwData[i]     = this->u.list.pdwData[i+1];
          this->u.list.ppStrs[i]      = this->u.list.ppStrs[i+1];
        }
      }
      return true;
    }
    return false;
  }
  // ----------------- CLEAR LIST --------------------
  bool dialog::clearList()
  {
    if ( this && this->isList() )
    {
      for ( int i = 255; i >= 0; i-- )
        this->removeListEntry((BYTE)i);
      dialog *scroll = this->u.list.pScrlBar;
      if ( scroll )
      {
        scroll->u.scroll.nMax = scroll->u.scroll.nMin;
        scroll->hide();
        scroll->update();
      }
      return true;
    }
    return false;
  }
  // -------------- GET LIST COUNT -------------------
  BYTE dialog::getListCount()
  {
    if ( this && this->isList() )
      return this->u.list.bStrs;
    return 0;
  }
};
