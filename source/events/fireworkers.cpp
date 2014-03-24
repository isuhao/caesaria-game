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

#include "fireworkers.hpp"
#include "game/game.hpp"
#include "city/city.hpp"
#include "gfx/tilemap.hpp"
#include "objects/working.hpp"
#include "gfx/tile.hpp"
#include "core/foreach.hpp"

namespace events
{

GameEventPtr FireWorkers::create(TilePos center, unsigned int workers)
{
  FireWorkers* e = new FireWorkers();
  e->_center  = center;
  e->_workers = workers;
  GameEventPtr ret( e );
  ret->drop();
  return ret;
}

bool FireWorkers::_mayExec(Game& game, unsigned int time) const
{
  return true;
}

void FireWorkers::_exec(Game& game, unsigned int)
{
  Tilemap& tilemap = game.city()->getTilemap();
  const int defaultFireWorkersDistance = 40;

  for( int curRange=1; curRange < defaultFireWorkersDistance; curRange++ )
  {
    TilesArray perimetr = tilemap.getRectangle( _center - TilePos( curRange, curRange ),
                                                 _center + TilePos( curRange, curRange ) );
    foreach( tile, perimetr )
    {
      WorkingBuildingPtr wrkBuilding = ptr_cast<WorkingBuilding>( (*tile)->overlay() );
      if( wrkBuilding.isValid() )
      {
        int bldWorkersCount = wrkBuilding->numberWorkers();
        wrkBuilding->removeWorkers( _workers );
        _workers -= math::clamp<int>( bldWorkersCount, 0, _workers );
      }

      if( !_workers )
        return;
    }
  }
}

}
