// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    param_test_i.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "tao/corba.h"
#include "param_test_i.h"

// ********* class Coffee_i ****************
// Constructor

Coffee_i::Coffee_i (const char *name)
  : name_ (name)
{
}

// Destructor

Coffee_i::~Coffee_i (void)
{
}

// get attribute
Coffee::Desc *
Coffee_i::description (CORBA::Environment & /*env*/)
{
  Coffee::Desc *desc = new Coffee::Desc;
  desc->name = CORBA::string_dup (this->name_);
  return desc;
}

// set attribute
void
Coffee_i::description (const Coffee::Desc &description,
                       CORBA::Environment & /*env*/)
{
  this->name_ = CORBA::string_dup (description.name);
}


// ********* class Param_Test_i ****************

// Constructor

Param_Test_i::Param_Test_i (const char *coffee_name,
                            const char *)
  : obj_ (coffee_name)
{
}

// Destructor

Param_Test_i::~Param_Test_i (void)
{
}

// test shorts
CORBA::Short
Param_Test_i::test_short (CORBA::Short s1,
                          CORBA::Short &s2,
                          CORBA::Short_out s3,
                          CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  s2 = s1 * 2;
  s3 = s1 * 3;
  return s1 * 4;
}

// test unbounded strings. For return and out types, we return duplicates of
// the in string. For the inout, we append the same string to itself and send
// it back
char *
Param_Test_i::test_unbounded_string (const char *s1,
                                     char *&s2,
                                     CORBA::String_out s3,
                                     CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  char *retstr = CORBA::string_dup (s1);
  s3 = CORBA::string_dup (s1);
  char *tmp = CORBA::string_alloc (2*ACE_OS::strlen (s2));
  ACE_OS::sprintf (tmp, "%s%s", s2, s2);
  CORBA::string_free (s2);
  s2 = tmp;
  return retstr;
}

// test for fixed structures. Just copy the in parameter into all the others
Param_Test::Fixed_Struct
Param_Test_i::test_fixed_struct (const Param_Test::Fixed_Struct &s1,
                                 Param_Test::Fixed_Struct &s2,
                                 Param_Test::Fixed_Struct_out s3,
                                 CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  s2 = s1;
  s3 = s1;
  return s1;
}

// = Sequences

Param_Test::Long_Seq * Param_Test_i::test_long_sequence (
      const Param_Test::Long_Seq & s1,
      Param_Test::Long_Seq & s2,
      Param_Test::Long_Seq_out s3,
      CORBA::Environment &
     )
{
  Param_Test::Long_Seq
    *ret = new Param_Test::Long_Seq,
    *out = new Param_Test::Long_Seq;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Bounded_Long_Seq * Param_Test_i::test_bounded_long_sequence (
      const Param_Test::Bounded_Long_Seq & s1,
      Param_Test::Bounded_Long_Seq & s2,
      Param_Test::Bounded_Long_Seq_out s3,
      CORBA::Environment &
     )
{
  Param_Test::Bounded_Long_Seq
    *ret = new Param_Test::Bounded_Long_Seq,
    *out = new Param_Test::Bounded_Long_Seq;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::StrSeq *
Param_Test_i::test_strseq (const Param_Test::StrSeq &s1,
                           Param_Test::StrSeq &s2,
                           Param_Test::StrSeq_out s3,
                           CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::StrSeq
    *ret = new Param_Test::StrSeq,
    *out = new Param_Test::StrSeq;

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Bounded_StrSeq * Param_Test_i::test_bounded_strseq (
      const Param_Test::Bounded_StrSeq & s1,
      Param_Test::Bounded_StrSeq & s2,
      Param_Test::Bounded_StrSeq_out s3,
      CORBA::Environment &
     )
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Bounded_StrSeq
    *ret = new Param_Test::Bounded_StrSeq,
    *out = new Param_Test::Bounded_StrSeq;

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// test for struct sequences
Param_Test::StructSeq *
Param_Test_i::test_struct_sequence (const Param_Test::StructSeq &s1,
                                    Param_Test::StructSeq &s2,
                                    Param_Test::StructSeq_out s3,
                                    CORBA::Environment &)
{
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::StructSeq
    *ret = new Param_Test::StructSeq,

    *out = new Param_Test::StructSeq;

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Bounded_StructSeq * Param_Test_i::test_bounded_struct_sequence (
      const Param_Test::Bounded_StructSeq & s1,
      Param_Test::Bounded_StructSeq & s2,
      Param_Test::Bounded_StructSeq_out s3,
      CORBA::Environment &
     )
{
  Param_Test::Bounded_StructSeq
    *ret = new Param_Test::Bounded_StructSeq,
    *out = new Param_Test::Bounded_StructSeq;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Coffee_Mix * Param_Test_i::test_coffe_mix (
      const Param_Test::Coffee_Mix & s1,
      Param_Test::Coffee_Mix & s2,
      Param_Test::Coffee_Mix_out s3,
      CORBA::Environment &
     )
{
  Param_Test::Coffee_Mix
    *ret = new Param_Test::Coffee_Mix,
    *out = new Param_Test::Coffee_Mix;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

Param_Test::Bounded_Coffee_Mix * Param_Test_i::test_bounded_coffe_mix (
      const Param_Test::Bounded_Coffee_Mix & s1,
      Param_Test::Bounded_Coffee_Mix & s2,
      Param_Test::Bounded_Coffee_Mix_out s3,
      CORBA::Environment &
     )
{
  Param_Test::Bounded_Coffee_Mix
    *ret = new Param_Test::Bounded_Coffee_Mix,
    *out = new Param_Test::Bounded_Coffee_Mix;

  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}


// = end of sequences...

// test for variable structs
Param_Test::Var_Struct *
Param_Test_i::test_var_struct (const Param_Test::Var_Struct &s1,
                             Param_Test::Var_Struct &s2,
                             Param_Test::Var_Struct_out s3,
                             CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Var_Struct
    *ret = new Param_Test::Var_Struct,
    *out = new Param_Test::Var_Struct;

  // now copy all elements of s1 into the others
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// test for nested structs
Param_Test::Nested_Struct *
Param_Test_i::test_nested_struct (const Param_Test::Nested_Struct &s1,
                                  Param_Test::Nested_Struct &s2,
                                  Param_Test::Nested_Struct_out s3,
                                  CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Nested_Struct
    *ret = new Param_Test::Nested_Struct,
    *out = new Param_Test::Nested_Struct;

  // now copy all elements of s1 into the others
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// make a Coffee object
Coffee_ptr
Param_Test_i::make_coffee (CORBA::Environment &env)
{
  return this->obj_._this (env);
}

// test for object references
Coffee_ptr
Param_Test_i::test_objref (Coffee_ptr o1,
                           Coffee_ptr &o2,
                           Coffee_out o3,
                           CORBA::Environment &env)
{
  Coffee_ptr ret = Coffee::_nil ();

  TAO_TRY
    {
      Coffee_var myobj = obj_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (myobj->_is_equivalent (o1, env))
        {
          o2 = Coffee::_duplicate (myobj.in ());
          o3 = Coffee::_duplicate (myobj.in ());
          ret = Coffee::_duplicate (myobj.in ());
        }
      else
        {
          o2 = Coffee::_nil ();
          o3 = Coffee::_nil ();
        }
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      env.exception (TAO_TRY_ENV.exception ());
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      env.exception (TAO_TRY_ENV.exception ());
    }
  TAO_ENDTRY;

  return ret;
}

// test for typecodes
CORBA::TypeCode_ptr
Param_Test_i::test_typecode (CORBA::TypeCode_ptr t1,
                             CORBA::TypeCode_ptr &t2,
                             CORBA::TypeCode_out t3,
                             CORBA::Environment &env)
{
  // we simply assign t1 to the others
  CORBA::TypeCode_ptr retval = CORBA::TypeCode::_duplicate (t1);
  t2 = CORBA::TypeCode::_duplicate (t1);
  t3 = CORBA::TypeCode::_duplicate (t1);
  return retval;
}

// test for Anys
CORBA::Any *
Param_Test_i::test_any (const CORBA::Any &a1,
                        CORBA::Any &a2,
                        CORBA::Any_out a3,
                        CORBA::Environment &env)
{
  return 0;
}
