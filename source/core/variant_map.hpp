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

#ifndef __CAESARIA_VARIANT_MAP_H_INCLUDED__
#define __CAESARIA_VARIANT_MAP_H_INCLUDED__

#include "variant.hpp"
#include "delegate.hpp"

/**
 * @brief Simple [string,variant] map container
 */
class VariantMap
{
public:
  typedef std::map<std::string, Variant> Collection;
  typedef Delegate2<const std::string&, const Variant&> Visitor;

  /**
   * @brief default constructor
   */
  VariantMap();

  static const Variant defValue;

  /**
   * @brief copy constructor
   * @param parent container
   */
  VariantMap(const VariantMap& other);

  //proxy functions
  Collection::iterator begin() { return _data.begin(); }
  Collection::const_iterator begin() const { return _data.begin(); }
  Collection::iterator end() { return _data.end(); }
  Collection::const_iterator end() const { return _data.end(); }
  Collection::const_iterator find(const std::string& name) const { return _data.find(name); }
  Collection::iterator erase( Collection::iterator it );
  void erase( const std::string& name );
  bool empty() const { return _data.empty(); }
  void clear() { _data.clear(); }
  size_t size() const { return _data.size(); }

  /**
   * @brief will add values from other container
   * @param other container
   * @return ref to self
   */
  VariantMap& operator+=(const VariantMap& other);

  /**
   * @brief Do function for each element in container
   * @param function
   */
  void visitEach(Visitor visitor);

  Variant& operator[](const std::string& name);

  /**
   * @brief replace values from other cotainer
   * @param other container
   * @return ref to self
   */
  VariantMap& operator=(const VariantMap& other);

  /**
   * @brief find and return value by key or empty value
   * @param key
   * @param default value which returns if key not found
   * @return variant
   */
  Variant get(const std::string& name, const Variant& defaultVal=defValue) const;

  /**
   * @brief check this key contains in container
   * @param key
   * @return true if found
   */
  bool has(const std::string& name) const;

  /**
   * @brief Convert container to variant
   * @return variant
   */
  Variant toVariant() const;

private:
  Collection _data;
};

#endif // __CAESARIA_VARIANT_MAP_H_INCLUDED__
