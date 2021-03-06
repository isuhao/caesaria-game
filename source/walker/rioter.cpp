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
// Copyright 2012-2014 dalerank, dalerankn8@gmail.com

#include "rioter.hpp"
#include "objects/house.hpp"
#include "pathway/path_finding.hpp"
#include "constants.hpp"
#include "city/statistic.hpp"
#include "objects/house_spec.hpp"
#include "objects/constants.hpp"
#include "pathway/astarpathfinding.hpp"
#include "gfx/tilemap.hpp"
#include "objects/constants.hpp"
#include "pathway/pathway_helper.hpp"
#include "corpse.hpp"
#include "core/priorities.hpp"
#include "ability.hpp"
#include "core/variant_map.hpp"
#include "events/disaster.hpp"
#include "game/resourcegroup.hpp"
#include "core/variant.hpp"
#include "game/gamedate.hpp"
#include "walkers_factory.hpp"
#include "core/common.hpp"

using namespace gfx;
using namespace events;

REGISTER_CLASS_IN_WALKERFACTORY(walker::rioter, Rioter)
REGISTER_CLASS_IN_WALKERFACTORY(walker::indigeneRioter, NativeRioter)

class Rioter::Impl
{
public:
  typedef enum { searchHouse=0, go2destination, searchAnyBuilding,
                 destroyConstruction, go2anyplace, gooutFromCity, wait } State;
  HouseLevel::ID houseLevel;
  State state;
  object::GroupSet excludeGroups;

public:
  Pathway findTarget(PlayerCityPtr city, const ConstructionList& items, TilePos pos );
};

Rioter::Rioter(PlayerCityPtr city)
  : Human( city, walker::rioter ), _d( new Impl )
{
  addAbility( Illness::create( 0.3, 4) );
  excludeAttack( object::group::disaster );
}

void Rioter::_reachedPathway()
{
  Walker::_reachedPathway();

  switch( _d->state )
  {
  case Impl::go2destination:
    _animation().clear();
    _animation().load( ResourceGroup::citizen2, 455, 8 );
    _animation().load( ResourceGroup::citizen2, 462, 8, Animation::reverse );
    _setAction( acFight );
    _d->state = Impl::destroyConstruction;
  break;

  default: break;
  }
}

void Rioter::_updateThoughts()
{
  StringArray ret;
  ret << "##rioter_say_1##"
      << "##rioter_say_2##"
      << "##rioter_say_3##";

  setThinks( ret.random() );
}

void Rioter::timeStep(const unsigned long time)
{
  Walker::timeStep( time );

  switch( _d->state )
  {
  case Impl::searchHouse:
  {
    ConstructionList constructions = _city()->statistic().objects.find<Construction>( object::house );
    for( auto it=constructions.begin(); it != constructions.end(); )
    {
      auto house = (*it).as<House>();
      if( house->level() <= _d->houseLevel ) { it=constructions.erase( it ); }
      else { ++it; }
    }

    Pathway pathway = _d->findTarget( _city(), constructions, pos() );
    //find more expensive house, fire this!!!
    if( pathway.isValid() )
    {
      setPos( pathway.startPos() );
      setPathway( pathway );
      go();
      _d->state = Impl::go2destination;
    }
    else //not find house, try find any, that rioter can destroy
    {
      _d->state = Impl::searchAnyBuilding;
    }
  }
  break;

  case Impl::searchAnyBuilding:
  {
    ConstructionList constructions = _city()->statistic().objects.find<Construction>( object::house );

    for( auto it=constructions.begin(); it != constructions.end(); )
    {
      object::Type type = (*it)->type();
      object::Group group = (*it)->group();
      if( type == object::house || type == object::road
          || _d->excludeGroups.count( group ) > 0 ) { it=constructions.erase( it ); }
      else { it++; }
    }

    Pathway pathway = _d->findTarget( _city(), constructions, pos() );
    if( pathway.isValid() )
    {
      setPos( pathway.startPos() );
      setPathway( pathway );
      go();
      _d->state = Impl::go2destination;
    }
    else
    {
      _d->state = Impl::go2anyplace;
    }
  }
  break;

  case Impl::go2anyplace:
  {
    Pathway pathway = PathwayHelper::randomWay( _city(), pos(), 10 );

    if( pathway.isValid() )
    {
      setPathway( pathway );
      go();
      _d->state = Impl::go2destination;
    }
    else
    {
      die();
      _d->state = Impl::wait;
    }
  }
  break;

  case Impl::go2destination:
  case Impl::wait:
  break;

  case Impl::destroyConstruction:
  {
    if( game::Date::isDayChanged() )
    {
      ConstructionList constructions = _city()->statistic().objects
                                                           .find<Construction>( object::any, pos(), 1 );

      for( auto it=constructions.begin(); it != constructions.end(); )
      {
        if( (*it)->type() == object::road || _d->excludeGroups.count( (*it)->group() ) > 0  )
        { it=constructions.erase( it ); }
        else { ++it; }
      }

       if( constructions.empty() )
      {
        _animation().clear();
        _setAction( acMove );
        _d->state = Impl::searchHouse;
      }
      else
      {
        for( auto c : constructions )
        {
          c->updateState( pr::fire, 1 );
          c->updateState( pr::damage, 1 );
          if( c->state( pr::damage ) < 10 || c->state( pr::fire ) < 10 )
          {
            events::dispatch<Disaster>( c->tile(), Disaster::riots );
          }
          break;
        }
      }
    }
  }
  break;

  default: break;
  }
}

Rioter::~Rioter() {}

void Rioter::send2City( BuildingPtr bld )
{
  TilesArray tiles = bld->enterArea();
  if( tiles.empty() )
    return;

  setPos( tiles.random()->pos() );
  _d->houseLevel = HouseLevel::vacantLot;

  if( bld.is<House>() )
    _d->houseLevel = (HouseLevel::ID)bld.as<House>()->level();

  _d->state = Impl::searchHouse;

  attach();
}

bool Rioter::die()
{
  bool created = Walker::die();

  if( !created )
  {
    Corpse::create( _city(), pos(), ResourceGroup::citizen2, 447, 454 );
    return true;
  }

  return created;
}

void Rioter::save(VariantMap& stream) const
{
  Walker::save( stream );

  VARIANT_SAVE_ENUM_D( stream, _d, houseLevel )
  VARIANT_SAVE_ENUM_D( stream, _d, state )
}

void Rioter::load(const VariantMap& stream)
{
  Walker::load( stream );

  VARIANT_LOAD_ENUM_D( _d, houseLevel, stream )
  VARIANT_LOAD_ENUM_D( _d, state, stream )
}

int Rioter::agressive() const { return 1; }
void Rioter::excludeAttack(object::Group group) { _d->excludeGroups << group; }

Pathway Rioter::Impl::findTarget(PlayerCityPtr city, const ConstructionList& items, TilePos pos )
{
  if( !items.empty() )
  {
    auto constructions = items.random( 10 );
    Pathway pathway;
    for( auto c : constructions )
    {
      pathway = PathwayHelper::create( pos, c, PathwayHelper::allTerrain );
      if( pathway.isValid() )
      {
        return pathway;
      }
    }
  }

  return Pathway();
}

NativeRioter::NativeRioter(PlayerCityPtr city)
  : Rioter( city )
{
  _setType( walker::indigeneRioter );
  excludeAttack( object::group::native );
}
