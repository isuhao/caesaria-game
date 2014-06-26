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

#include "desirability_updater.hpp"
#include "game/game.hpp"
#include "objects/construction.hpp"
#include "helper.hpp"
#include "city.hpp"
#include "game/gamedate.hpp"
#include "objects/house.hpp"
#include "core/logger.hpp"
#include "events/dispatcher.hpp"
#include "gfx/tilemap.hpp"

using namespace constants;
using namespace gfx;

namespace city
{

class DesirabilityUpdater::Impl
{
public:
  DateTime endTime;
  bool isDeleted;
  int value;
  bool alsoInfluence;

  void update(PlayerCity& city, bool positive );
};

SrvcPtr DesirabilityUpdater::create( PlayerCityPtr city )
{
  DesirabilityUpdater* e = new DesirabilityUpdater( city );

  SrvcPtr ret( e );
  ret->drop();

  return ret;
}

void DesirabilityUpdater::update( const unsigned int time)
{
  if( GameDate::isMonthChanged() )
  {
    _d->isDeleted = (_d->endTime < GameDate::current());

    if( !_d->alsoInfluence )
    {      
      _d->alsoInfluence = true;
      _d->update( _city, true );
    }
  }
}

std::string DesirabilityUpdater::defaultName() { return "desirability_updater"; }
bool DesirabilityUpdater::isDeleted() const {  return _d->isDeleted; }
void DesirabilityUpdater::destroy(){ _d->update( _city, false );}

void DesirabilityUpdater::load(const VariantMap& stream)
{
  _d->endTime = stream.get( "endTime" ).toDateTime();
  _d->value = stream.get( "value" );
  _d->alsoInfluence = stream.get( "alsoInfluence", false );
}

VariantMap DesirabilityUpdater::save() const
{
  VariantMap ret;
  ret[ "endTime" ] = _d->endTime;
  ret[ "value" ] = _d->value;
  ret[ "alsoInfluence" ] = _d->alsoInfluence;

  return ret;
}

DesirabilityUpdater::DesirabilityUpdater( PlayerCityPtr city ) : Srvc( *city.object(), DesirabilityUpdater::defaultName() ), _d( new Impl )
{
  _d->isDeleted = false;
  _d->alsoInfluence = false;
}

void DesirabilityUpdater::Impl::update( PlayerCity& city, bool positive)
{
  int size = city.tilemap().size();
  TilesArray tiles = city.tilemap().getArea( TilePos( 0, 0), Size( size ) );

  foreach( it, tiles )
  {
    (*it)->appendDesirability( positive ? value : -value);
  }
}

}//end namespace city
