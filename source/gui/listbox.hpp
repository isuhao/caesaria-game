// This file is part of CaesarIA.
//
// CaesarIA is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CaesarIA is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CaesarIA.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2012-2016 Dalerank, dalerankn8@gmail.com

#ifndef __CAESARIA_GUI_LISTBOX_H_INCLUDED__
#define __CAESARIA_GUI_LISTBOX_H_INCLUDED__

#include "gui/widget.hpp"
#include "core/flagholder.hpp"
#include "listboxitem.hpp"
#include "core/predefinitions.hpp"
#include "core/signals.hpp"
#include "core/stringarray.hpp"
#include "font/font.hpp"

namespace gui
{

class ScrollBar;

class ListBox : public Widget, public FlagHolder<int>
{
public:
  typedef enum { selectOnMove=0x1, selectOnMDown=0x2,
                 drawBackground=0x4, autoscroll=0x8, moveOverSelect=0x10,
                 hightlightNotinfocused=0x20, itemSelectable=0x40 } Flag;

  typedef enum { selectOnMouseMove=true, selectOnClick=false } SelectMode;

  //! constructor
  ListBox( Widget* parent,
           const Rect& rectangle=Rect( 0, 0, 1, 1), int id=-1, bool clip=true,
           bool drawBack=false, bool mos=false);

  //! constructor
  ListBox(Widget* parent,
          const RectF& rectangle, int id=-1, bool clip=true,
          bool drawBack=false, bool mos=false);

  //! destructor
  virtual ~ListBox();

  //! returns amount of list items
  virtual unsigned int itemsCount() const;

  //! returns string of a list item. the id may be a value from 0 to itemCount-1
  virtual ListBoxItem& itemAt(unsigned int index);

  //!
  virtual ListBoxItem& selectedItem();

  //! clears the list
  virtual void clear();

  //! sets the selected item. Set this to -1 if no item should be selected
  virtual void setSelected(int id);

  //!
  virtual int  findItem(const std::string& text) const;

  //!
  virtual void setSelectedTag(const Variant& tag);

  //!
  virtual void setSelectedWithData(const std::string& name, const Variant& data);

  //! sets the selected item. Set this to -1 if no item should be selected
  virtual void setSelected(const std::string &item);

  //! called if an event happened.
  virtual bool onEvent(const NEvent& event);

  //! draws the element and its children
  virtual void draw(gfx::Engine& painter);

  //! removes an item from the list
  virtual void removeItem(unsigned int id);

  //! get the the id of the item at the given absolute coordinates
  virtual int findIndex(Point pos) const;

  //! set all item colors of specified type at given index to color
  virtual void setItemOverrideColor(unsigned int index, NColor color, int colorType=ListBoxItem::all);

  //! set whether the listbox should scroll to newly selected items
  virtual void setAutoScrollEnabled(bool scroll);

  //!
  void setScrollbarVisible(bool visible);

  //! returns true if automatic scrolling is enabled, false if not.
  virtual bool isAutoScrollEnabled() const;

  //! clear all item colors at index
  virtual void resetItemOverrideColor(unsigned int index);

  //!
  void setItemEnabled(unsigned int index, bool enabled);

  //! clear item color at index for given colortype
  virtual void resetItemOverrideColor(unsigned int index, ListBoxItem::ColorType colorType);

  //! has the item at index its color overwritten?
  virtual bool hasItemOverrideColor(unsigned int index, ListBoxItem::ColorType colorType) const;

  //! return the overwrite color at given item index.
  virtual NColor getItemOverrideColor(unsigned int index, ListBoxItem::ColorType colorType) const;

  //! return the default color which is used for the given colorType
  virtual NColor itemDefaultColor( ListBoxItem::ColorType colorType) const;

  //! set default color which will used for the given colorType
  virtual void setItemDefaultColor( ListBoxItem::ColorType colorType, const NColor& color);

  //! set default color which will used for the given colorType
  virtual void setItemDefaultColor(const std::string& typeName, const std::string& colorName);

  //!
  virtual void setItemsFont(Font font);

  //!
  virtual void setItemsFont( const std::string& fontname );

  //!
  virtual void setItemTooltip(unsigned int index, const std::string& text);

  //!
  void setItemsSelectable(bool en);

  //! set the item at the given index
  virtual void setItemText(unsigned int index,const std::string& text);

  //! set item icon
  virtual void setItemIcon(unsigned int index, gfx::Picture icon, const Point& offset);

  //!
  virtual void setItemData(unsigned int index, const std::string& name, Variant tag);

  //!
  virtual Variant getItemData(unsigned int index, const std::string& name);

  //!
  virtual Rect getItemRectangle(unsigned int index);

  //! Insert the item at the given index
  //! Return the index on success or -1 on failure.
  virtual int insertItem(unsigned int index, std::string text);

  //! Swap the items at the given indices
  virtual void swapItems(unsigned int index1, unsigned int index2);

  //! set global itemHeight
  virtual void setItemsHeight(int height);

  //!
  virtual int itemsHeight() const;

  //!
  virtual void setItemAlignment(int index, Alignment horizontal, Alignment vertical);

  //! Sets whether to draw the background
  virtual void setBackgroundVisible(bool draw);

  //! adds an list item
  //! \param text Text of list entry
  //! \param font
  //! \param color
  //! \return
  //! returns the id of the new created item
  virtual ListBoxItem& addItem(const std::string& text, Font font=Font(), NColor color=NColor() );

  //!
  virtual ListBoxItem& addItem(gfx::Picture pic);

  //!
  virtual int addLine(const std::string& text);

  //!
  virtual void fitText(const std::string& text);

  //!
  virtual void addLines(const StringArray& strings);

  //!
  virtual Font font() const;

  //!
  virtual void setMargin(int type, int value);

  //!
  virtual int selected();

  //!
  virtual void beforeDraw(gfx::Engine& painter);

  //!
  virtual void refresh();

  //!
  virtual void setItemsTextOffset(Point p);

  //!
  virtual void setupUI(const VariantMap &ui);
  virtual void setupUI(const vfs::Path& path);

signals public:
  Signal1<const ListBoxItem&>& onItemSelectedAgain();
  Signal1<const ListBoxItem&>& onItemSelected();

  Signal2<Widget*,int>& onIndexSelectedEx();
  Signal2<Widget*,int>& onIndexSelectedAgainEx();
  Signal2<Widget*,int>& onIndexRmbClickedEx();

protected:
  //! Update the position and size of the listbox, and update the scrollbar
  virtual void _finalizeResize();
  virtual void _drawItemIcon(gfx::Engine& painter, ListBoxItem& item, const Point& pos, Rect* clipRect);
  virtual void _drawItemText(gfx::Engine& painter, ListBoxItem& item, const Point& pos, Rect* clipRect);
  virtual void _updateItemText(gfx::Engine& painter, ListBoxItem& item, const Rect& textRect, Font font, const Rect& frameRect );
  Rect _itemsRect();

private:
  void _selectNew(int ypos);
  void _recalculateScrollPos();
  void _updateBackground(int scrollbarWidth);
  void _recalculateItemHeight( const Font& defaulFont, int height );
  void _indexChanged( unsigned int eventType );
  ElementState _getCurrentItemState( unsigned int index, bool hl );
  Font _getCurrentItemFont( const ListBoxItem& item, bool selected );
  NColor _getCurrentItemColor( const ListBoxItem& item, bool selected );

  class Impl;
  ScopedPtr< Impl > _d;
};

}//end namespace gui
#endif //__CAESARIA_GUI_LISTBOX_H_INCLUDED__
