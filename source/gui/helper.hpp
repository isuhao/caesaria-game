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
// Copyright 2012-2014 Dalerank, dalerankn8@gmail.com

#ifndef __CAESARIA_WIDGET_HELPER_H_INCLUDED__
#define __CAESARIA_WIDGET_HELPER_H_INCLUDED__

#include "label.hpp"
class Walker;

namespace gui
{

class SoundMuter : public Widget
{
public:
  SoundMuter(Widget* parent, int value=-1);
  ~SoundMuter();

  void setVolume(int value);
private:
  std::map<int,int> _states;
};

class SoundEmitter : public Widget
{
public:
  SoundEmitter(Widget* parent, const std::string& sound="", int volume=0, int type=0);
  ~SoundEmitter();
  void assign(const std::string& sampleName, int volume, int type);

private:
  std::string _sample;
};

class WalkerImage : public Label
{
public:
  WalkerImage(Widget* parent, const Rect& rectangle=Rect(), SmartPtr<Walker> wlk=nullptr);

  virtual void _handleClick();

  virtual void draw(gfx::Engine &painter);

  SmartPtr<Walker> _walker;
  gfx::Picture _wlkPicture;
  Signal1<SmartPtr<Walker>> _onClickedSignal;
};

class ImageBlinker : public Widget
{
public:
  ImageBlinker(Widget* parent, const Rect& rect=Rect());
  void setImage(gfx::Picture pic);
  void setActive(bool active);

  virtual void setProperty(const std::string &name, const Variant &value);
  virtual void draw(gfx::Engine& painter);

private:
  bool _active;
  gfx::Picture _pic;
};

}//end namesapce gui
#endif //__CAESARIA_WIDGET_HELPER_H_INCLUDED__
