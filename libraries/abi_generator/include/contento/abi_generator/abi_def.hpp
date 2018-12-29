/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

//#include <contento/abi_generator/types.hpp>
//#include <fc/io/varint.hpp>
//#include <fc/reflect/variant.hpp>
//#include <fc/variant_object.hpp>
//#include <fc/exception/exception.hpp>

#include <map>
#include <string>
#include <vector>
#include <set>
//#include <optional>
#include <boost/assert.hpp>
#include <iostream>
#include <boost/property_tree/ptree.hpp>

#define FC_ASSERT(x, ...) BOOST_ASSERT(x);
#define FC_CAPTURE_AND_RETHROW(...)  \
   catch( ... ) {  \
      throw std::current_exception(); \
   }
#define wlog(x, ...) std::cout << x << std::endl
#define FC_CAPTURE_AND_LOG(...)   \
   catch( ... ) {  \
   }

namespace contento { namespace chain {
   using boost::property_tree::ptree;

   using std::vector;
   using std::string;
   //using std::map;

   using type_name      = std::string;
   using field_name     = std::string;
   using table_name = string;
   using action_name = string;
   using name = string;

struct type_def {
   type_def() = default;
   type_def(const type_name& new_type_name, const type_name& type)
   :new_type_name(new_type_name), type(type)
   {}

   type_name   new_type_name;
   type_name   type;

   void to_json(ptree& out){
      out.put("new_type_name", new_type_name.c_str());
      out.put("type", type.c_str());
   }
};

struct field_def {
   field_def() = default;
   field_def(const field_name& name, const type_name& type)
   :name(name), type(type)
   {}

   field_name name;
   type_name  type;

   bool operator==(const field_def& other) const {
      return std::tie(name, type) == std::tie(other.name, other.type);
   }

   void to_json(ptree& out){
      out.put("name", name.c_str());
      out.put("type", type.c_str());
   }
};

struct struct_def {
   struct_def() = default;
   struct_def(const type_name& name, const type_name& base, const vector<field_def>& fields)
   :name(name), base(base), fields(fields)
   {}

   type_name            name;
   type_name            base;
   vector<field_def>    fields;

   bool operator==(const struct_def& other) const {
      return std::tie(name, base, fields) == std::tie(other.name, other.base, other.fields);
   }

   void to_json(ptree& out){
      out.put("name", name.c_str());
      out.put("base", base.c_str());

      ptree ofields;
      for(auto i : fields){
         ptree ofield;
         i.to_json(ofield);
         ofields.push_back(std::make_pair("", ofield));
      }
      out.add_child("fields", ofields);
   }
};

struct action_def {
   action_def() = default;
   action_def(const action_name& name, const type_name& type, const string& ricardian_contract)
   :name(name), type(type), ricardian_contract(ricardian_contract)
   {}

   action_name name;
   type_name   type;
   string      ricardian_contract;

   void to_json(ptree& out){
      out.put("name", name.c_str());
      out.put("type", type.c_str());
   }
};

struct table_def {
   table_def() = default;
   table_def(const table_name& name, const type_name& index_type, const vector<field_name>& key_names, const vector<type_name>& key_types, const type_name& type)
   :name(name), index_type(index_type), key_names(key_names), key_types(key_types), type(type)
   {}

   table_name         name;        // the name of the table
   type_name          index_type;  // the kind of index, i64, i128i128, etc
   vector<field_name> key_names;   // names for the keys defined by key_types
   vector<type_name>  key_types;   // the type of key parameters
   type_name          type;        // type of binary data stored in this table
};

struct clause_pair {
   clause_pair() = default;
   clause_pair( const string& id, const string& body )
   : id(id), body(body)
   {}

   string id;
   string body;
};

struct error_message {
   error_message() = default;
   error_message( uint64_t error_code, const string& error_msg )
   : error_code(error_code), error_msg(error_msg)
   {}

   uint64_t error_code;
   string   error_msg;
};

struct abi_def {
   abi_def() = default;
   abi_def(const vector<type_def>& types, const vector<struct_def>& structs, const vector<action_def>& actions, const vector<table_def>& tables, const vector<clause_pair>& clauses, const vector<error_message>& error_msgs)
   :version("contento::abi/1.0")
   ,types(types)
   ,structs(structs)
   ,actions(actions)
   ,tables(tables)
   ,ricardian_clauses(clauses)
   ,error_messages(error_msgs)
   {}

   string                version = "contento::abi/1.0";
   vector<type_def>      types;
   vector<struct_def>    structs;
   vector<action_def>    actions;
   vector<table_def>     tables;
   vector<clause_pair>   ricardian_clauses;
   vector<error_message> error_messages;
   //extensions_type       abi_extensions;

   void to_json(ptree& out){
      out.put("version", version.c_str());

      {
         ptree obtrees;
         for(auto i : types){
            ptree otype;
            i.to_json(otype);
            obtrees.push_back(std::make_pair("", otype));
         }
         out.add_child("types", obtrees);
      }
      {
         ptree obtrees;
         for(auto i : structs){
            ptree obtree;
            i.to_json(obtree);
            obtrees.push_back(std::make_pair("", obtree));
         }
         out.add_child("structs", obtrees);
      }
      {
         ptree obtrees;
         for(auto i : actions){
            ptree obtree;
            i.to_json(obtree);
            obtrees.push_back(std::make_pair("", obtree));
         }
         out.add_child("actions", obtrees);
      }
   }
};

abi_def contento_contract_abi(const abi_def& contento_system_abi);
vector<type_def> common_type_defs();

} } /// namespace contento::chain

//FC_REFLECT( contento::chain::type_def                         , (new_type_name)(type) )
//FC_REFLECT( contento::chain::field_def                        , (name)(type) )
//FC_REFLECT( contento::chain::struct_def                       , (name)(base)(fields) )
//FC_REFLECT( contento::chain::action_def                       , (name)(type)(ricardian_contract) )
//FC_REFLECT( contento::chain::table_def                        , (name)(index_type)(key_names)(key_types)(type) )
//FC_REFLECT( contento::chain::clause_pair                      , (id)(body) )
//FC_REFLECT( contento::chain::error_message                    , (error_code)(error_msg) )
//FC_REFLECT( contento::chain::abi_def                          , (version)(types)(structs)(actions)(tables)
//                                                             (ricardian_clauses)(error_messages) )
