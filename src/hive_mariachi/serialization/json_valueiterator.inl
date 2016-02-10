// Hive Mariachi Engine
// Copyright (c) 2008-2016 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the Apache License as published by the Apache
// Foundation, either version 2.0 of the License, or (at your option) any
// later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Apache License for more details.
//
// You should have received a copy of the Apache License along with
// Hive Mariachi Engine. If not, see <http://www.apache.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008-2016 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

// included by json_value.cpp
// everything is within Json namespace

ValueIteratorBase::ValueIteratorBase() : current_(), isNull_( true ) {
}

ValueIteratorBase::ValueIteratorBase(const Value::ObjectValues::iterator &current) : current_( current ), isNull_( false ) {
}

Value & ValueIteratorBase::deref() const {
	return current_->second;
}

void ValueIteratorBase::increment() {
	++current_;
}

void ValueIteratorBase::decrement() {
	--current_;
}

ValueIteratorBase::difference_type ValueIteratorBase::computeDistance( const SelfType &other ) const
{
   // Iterator for null value are initialized using the default
   // constructor, which initialize current_ to the default
   // std::map::iterator. As begin() and end() are two instance 
   // of the default std::map::iterator, they can not be compared.
   // To allow this, we handle this comparison specifically.
   if ( isNull_  &&  other.isNull_ )
   {
      return 0;
   }


   // Usage of std::distance is not portable (does not compile with Sun Studio 12 RogueWave STL,
   // which is the one used by default).
   // Using a portable hand-made version for non random iterator instead:
   //   return difference_type( std::distance( current_, other.current_ ) );
   difference_type myDistance = 0;
   for ( Value::ObjectValues::iterator it = current_; it != other.current_; ++it )
   {
      ++myDistance;
   }
   return myDistance;
}

bool ValueIteratorBase::isEqual( const SelfType &other ) const {
   if(isNull_) {
      return other.isNull_;
   }

   return current_ == other.current_;
}

void ValueIteratorBase::copy(const SelfType &other) {
   current_ = other.current_;
}


Value ValueIteratorBase::key() const {
   const Value::CZString czstring = (*current_).first;
   if ( czstring.c_str() )
   {
      if ( czstring.isStaticString() )
         return Value( StaticString( czstring.c_str() ) );
      return Value( czstring.c_str() );
   }
   return Value( czstring.index() );
}


Value::UInt 
ValueIteratorBase::index() const
{
   const Value::CZString czstring = (*current_).first;
   if ( !czstring.c_str() )
      return czstring.index();
   return Value::UInt( -1 );
}


const char *
ValueIteratorBase::memberName() const
{
#ifndef JSON_VALUE_USE_INTERNAL_MAP
   const char *name = (*current_).first.c_str();
   return name ? name : "";
#else
   if ( !isArray_ )
      return ValueInternalMap::key( iterator_.map_ );
   return "";
#endif
}


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class ValueConstIterator
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

ValueConstIterator::ValueConstIterator()
{
}


#ifndef JSON_VALUE_USE_INTERNAL_MAP
ValueConstIterator::ValueConstIterator( const Value::ObjectValues::iterator &current )
   : ValueIteratorBase( current )
{
}
#else
ValueConstIterator::ValueConstIterator( const ValueInternalArray::IteratorState &state )
   : ValueIteratorBase( state )
{
}

ValueConstIterator::ValueConstIterator( const ValueInternalMap::IteratorState &state )
   : ValueIteratorBase( state )
{
}
#endif

ValueConstIterator &
ValueConstIterator::operator =( const ValueIteratorBase &other )
{
   copy( other );
   return *this;
}


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class ValueIterator
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

ValueIterator::ValueIterator()
{
}


#ifndef JSON_VALUE_USE_INTERNAL_MAP
ValueIterator::ValueIterator( const Value::ObjectValues::iterator &current )
   : ValueIteratorBase( current )
{
}
#else
ValueIterator::ValueIterator( const ValueInternalArray::IteratorState &state )
   : ValueIteratorBase( state )
{
}

ValueIterator::ValueIterator( const ValueInternalMap::IteratorState &state )
   : ValueIteratorBase( state )
{
}
#endif

ValueIterator::ValueIterator(const ValueConstIterator &other) : ValueIteratorBase(other) {
}

ValueIterator::ValueIterator(const ValueIterator &other) : ValueIteratorBase(other) {
}

ValueIterator &ValueIterator::operator =(const SelfType &other) {
   copy(other);
   return *this;
}
