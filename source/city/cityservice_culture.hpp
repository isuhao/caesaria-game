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
// Copyright 2012-2013 Dalerank, dalerankn8@gmail.com

#ifndef __CAESARIA_CITYSERVICE_CULTURE_H_INCLUDED__
#define __CAESARIA_CITYSERVICE_CULTURE_H_INCLUDED__

#include "cityservice.hpp"
#include "game/predefinitions.hpp"

namespace city
{

PREDEFINE_CLASS_SMARTPOINTER(CultureRating)

class CultureRating : public Srvc
{
public:
  typedef enum { covSchool=0, covLibrary, covAcademy, covReligion, covTheatres, covCount } Coverage;

  virtual void timeStep( const unsigned int time );
  virtual VariantMap save() const;
  virtual void load(const VariantMap& stream);
  int value() const;

  int coverage( Coverage type ) const;
  int objects_n( Coverage type ) const;

  static std::string defaultName();

  CultureRating(PlayerCityPtr city);
private:

  class Impl;
  ScopedPtr< Impl > _d;
};

}//end namespace city

#endif //__CAESARIA_CITYSERVICE_CULTURE_H_INCLUDED__
