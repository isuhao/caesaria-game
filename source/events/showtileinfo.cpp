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

#include "showtileinfo.hpp"
#include "game/infoboxmanager.hpp"
#include "game/game.hpp"
#include "city/city.hpp"

namespace events
{

GameEventPtr ShowTileInfo::create( TilePos pos )
{
  ShowTileInfo* e = new ShowTileInfo();
  e->_pos = pos;

  GameEventPtr ret( e );
  ret->drop();

  return ret;
}

void ShowTileInfo::_exec(Game& game, uint time)
{
  InfoboxManager::getInstance().showHelp( game.getCity(), game.getGui(), _pos );
}

bool ShowTileInfo::_mayExec(Game&, uint ) const { return true; }

}
