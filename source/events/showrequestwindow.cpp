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

#include "showrequestwindow.hpp"
#include "game/game.hpp"
#include "city/city.hpp"
#include "gui/requestwindow.hpp"
#include "gui/environment.hpp"

namespace events
{

GameEventPtr ShowRequestInfo::create( city::request::RequestPtr request, bool available4exec)
{
  ShowRequestInfo* e = new ShowRequestInfo();
  e->_request = request;
  e->_reqAvailable = available4exec;

  GameEventPtr ret( e );
  ret->drop();

  return ret;
}

bool ShowRequestInfo::_mayExec(Game&, unsigned int) const {  return true; }

void ShowRequestInfo::_exec(Game& game, unsigned int)
{
  if( _request.isValid() )
  {
    gui::EmperrorRequestWindow* wnd = gui::EmperrorRequestWindow::create( game.gui()->rootWidget(), _request, _reqAvailable );
    wnd->show();
  }
}

}
