//----------------------------------* C++ *----------------------------------//
/*!
 * \file    rng/TF_Gen.hh
 * \author  Peter Ahrens
 * \brief   Declaration of class TF_Gen (modeled after LF_Gen)
 */
//---------------------------------------------------------------------------//

#ifndef TF_Gen_hh
#define TF_Gen_hh

#include "Random123/threefry.h"
#include "Random123/array.h"
#include <ds++/Data_Table.hh>
#include <ds++/ArrayWrap.hh>
#include <algorithm>
#include <stdint.h>
#include <math.h>

namespace rtt_rng
{

  typedef r123::Threefry2x32 TFRNG;

  //forward declaration
  class TF_Gen;

  //*! This is a reference to a TF_Gen */
  class TF_Gen_Ref
  {
  public:
    TF_Gen_Ref(unsigned int* const db, unsigned int* const de)
      : data(db, de)
    { Require(std::distance(db,de) == 4); }

    double ran() const
    {
      TFRNG aynRand;
      TFRNG::ctr_type ctr = {{data[0], data[1]}};
      TFRNG::key_type key = {{data[2], data[3]}};
      TFRNG::ctr_type result = aynRand(ctr, key);
      ctr.incr();
      std::copy(ctr.data(), ctr.data() + 2, data.access());
      return ((double)assemble_from_u32<uint64_t>(result.data())) / pow(2, 64);
    }

    //! Spawn a new, independent stream from this one (included for compatibility)
    inline void spawn(TF_Gen& new_gen) const;

    //! Return the identifier number for this stream
    uint64_t get_num() const {
      return (assemble_from_u32<uint64_t>(data.access() + 2)); }

    //! Return a unique number for this stream and state
    // unsure on how to return the necessary 128-bit precision in a <64-bit integer
    unsigned int get_unique_num() const { return 0xdeadbeef;}


    inline bool is_alias_for(TF_Gen const &rng);

  private:
    mutable rtt_dsxx::Data_Table<unsigned int> data;
  };

//===========================================================================//
/*!
 * \class TF_Gen
 * \brief This holds the data for, and acts as the interface to, one random
 *        number stream
 */
//===========================================================================//

  class TF_Gen
  {
  private:

    friend class TF_Gen_Ref;
    mutable unsigned int data[4];

  public:

    typedef unsigned int* iterator;
    typedef unsigned int const * const_iterator;

    //! Default constructor
    TF_Gen() { Require(sizeof(data)/sizeof(unsigned int) == 4); }

    TF_Gen(uint32_t const seed, uint32_t const streamnum)
    {
      data[2] = seed;
      data[3] = streamnum;
    }

    TF_Gen(unsigned int* const _data)
    {
      std::copy(_data, _data + 4, data);
    }

    //! (included for compatibility)
    void finish_init() const {}

    //! Return a random double
    double ran() const
    {
      TFRNG aynRand;
      TFRNG::ctr_type ctr = {{data[0], data[1]}};
      TFRNG::key_type key = {{data[2], data[3]}};
      TFRNG::ctr_type result = aynRand(ctr, key);
      ctr.incr();
      std::copy(ctr.data(), ctr.data() + 2, data);
      return ((double)assemble_from_u32<uint64_t>(result.data())) / pow(2, 64);
    }

    //! Spawn a new, independent stream from this one.
    void spawn(TF_Gen& new_gen) const
    {
      new_gen.data[0] = 0;
      new_gen.data[1] = 0;
      new_gen.data[2] = data[2];
      new_gen.data[3] = data[3] + 1;
    }

    //! Return the identifier number for this stream
    uint64_t get_num() const {
      return (assemble_from_u32<uint64_t>(data + 2)); }

    //! Return a unique number for this stream and state
    // unsure on how to return the necessary 128-bit precision in a <64-bit integer
    unsigned int get_unique_num() const { return 0xdeadbeef;}

    //! Return the size of the state
    unsigned int size() const { return 4; }

    iterator begin() { return data; }

    iterator end() { return data + 4; }

    const_iterator begin() const { return data; }

    const_iterator end() const { return data + 4; }

    bool operator==(TF_Gen const & rhs) const {
      return std::equal(begin(), end(), rhs.begin()); }

    TF_Gen_Ref ref() const {
      return TF_Gen_Ref(data, data + 4); }

    static unsigned int size_bytes() {
      return 4*sizeof(unsigned int); }

#if 0
    // Copying RNG streams shouldn't be done lightly!
    inline TF_Gen& operator=(TF_Gen const &src)
    {
      if(&src != this)
	std::memcpy(data, src.data, size_bytes());
      return *this;
    }
#endif

  private:
    TF_Gen(TF_Gen const &);

  };
//---------------------------------------------------------------------------//
// Implementation
//---------------------------------------------------------------------------//

  // This implementation requires the full definition of TF_Gen, so it must be
  // placed here instead of in the TF_Gen_Ref class.

  inline void TF_Gen_Ref::spawn(TF_Gen& new_gen) const
  {
    new_gen.data[0] = 0;
    new_gen.data[1] = 0;
    new_gen.data[2] = data[2];
    new_gen.data[3] = data[3] + 1;
  }

  inline bool TF_Gen_Ref::is_alias_for(TF_Gen const &rng)
  {
    return rng.begin() == data.access(); }

} // end namespace rtt_rng
#endif