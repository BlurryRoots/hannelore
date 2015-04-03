#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include <sstream>
#include <cassert>
#include <typeinfo>
#include <typeindex>
#include <set>
#include <type_traits>

#include <yanecos/IData.h>
#include <yanecos/Data.h>

#include <json.hpp>

namespace Yanecos {

typedef
	unsigned long long int
	EntityID
	;
typedef
	unsigned long long int
	DataID
	;

typedef
	//std::unordered_map<DataID, std::shared_ptr<IData>>
	std::unordered_map<DataID, IData*>
	DataMap
	;
typedef
	std::unordered_set<DataID>
	EntityDataCollection
	;
typedef
	std::unordered_map<EntityID, EntityDataCollection>
	EntityDataMap
	;
typedef
	std::unordered_set<EntityID>
	EntityCollection
	;
typedef
	std::unordered_map<std::string, EntityCollection>
	DataOwnerMap
	;
typedef
	std::unordered_map<std::string, EntityDataCollection>
	DataTypeLookupMap
	;

// Template array of bool
template<bool... TBool>
struct BoolArray {};

// Template checklist
template<bool... TBool>
struct all_types_derived
: std::is_same<BoolArray<TBool...>, BoolArray<(TBool, true)...>> {};

// Template check for data type inheritance
template<class... TDataType>
struct all_derived_from_data
: all_types_derived<std::is_base_of<Data<TDataType>, TDataType>::value...> {};

class EntityManager {

private:
	DataMap data;
	DataOwnerMap data_owner;
	DataTypeLookupMap type_lookup;

	DataID data_id_counter;
	DataID generate_data_id () {
		return ++this->data_id_counter;
	}

	EntityDataMap entity_data;

	EntityID entity_id_counter;
	EntityID generate_entity_id () {
		return ++this->entity_id_counter;
	}

public:
	EntityManager ()
	: data ()
	, data_owner ()
	, type_lookup ()
	, data_id_counter (0)
	, entity_data ()
	, entity_id_counter (0) {
		//
	};

	virtual
	~EntityManager () {
	}

	void
	dispose () {
#ifdef DEBUG_SERIALIZE
		std::cout << this->serialize () << std::endl << std::endl;
#endif

		for (auto entry : this->data) {
			delete entry.second;
		}
		this->data.clear ();

		this->data_owner.clear ();

		this->type_lookup.clear ();

		this->entity_data.clear ();
	}

	std::string
	serialize () {
		using json = nlohmann::json;

		json j;

		j["data_owner"] = json (this->data_owner);
		j["type_lookup"] = json (this->type_lookup);

		json entity_data_json = json::array();
		for (auto &entry : this->entity_data) {
			json jentry = json::object ();
			jentry[std::to_string (entry.first)] = json (entry.second);
			entity_data_json.push_back (jentry);
		}
		j["entity_data"] = entity_data_json;

		return j.dump ();
	}

	EntityID
	create_entity (void) {
		EntityID id = this->generate_entity_id ();

		// Check if emplacing collided
		const auto &report = this->entity_data.emplace (id, EntityDataCollection ());
		if (! report.second) {
			std::stringstream ss;
			ss << "Entity " << id << " already exists!";

			throw std::runtime_error (ss.str ());
		}

		return id;
	}

	template<class TDataType> TDataType*
	get_entity_data (EntityID entity_id) const {
		static_assert (
			all_derived_from_data<TDataType>::value,
			"Given type has to be derived from Data<>!"
		);

		// Fetch type name
		const auto &type_name = typeid (TDataType).name ();

		// Get all data contained in entity
		const auto &collection = this->entity_data.at (entity_id);
		if (0 == collection.size ()) {
			std::stringstream ss;
			ss << "Entity " << entity_id << " has no data!";

			throw std::runtime_error (ss.str ());
		}

		for (auto &data_id : collection) {
			std::string cur = this->data.at (data_id)->get_type ();
			if (cur == type_name) {
				return dynamic_cast<TDataType*> (this->data.at (data_id));
			}
		}

		std::stringstream ss;
		ss << "No data of type " << type_name;
		throw std::runtime_error (ss.str ());
	}

	template<class TDataType, class... TArgs> TDataType*
	add_data (EntityID entity_id, TArgs&&... args) {
		static_assert (
			all_derived_from_data<TDataType>::value,
			"Given type has to be derived from Data<>!"
		);

		const auto &type_name = typeid (TDataType).name ();

		if (0 == this->data_owner.count (type_name)) {
			this->data_owner.emplace (type_name, EntityCollection ());
		}

		auto &owners = this->data_owner.at (type_name);
		if (owners.end () != owners.find (entity_id)) {
			std::stringstream ss;
			ss << "Entity " << entity_id << " already has " << type_name << "!";

			throw std::runtime_error (ss.str ());
		}
		owners.emplace (entity_id);

		auto data_ptr = new TDataType (std::forward<TArgs> (args)...);
		assert (data_ptr);

		DataID data_id = this->generate_data_id ();
		assert (0 == this->data.count (data_id));

		auto report = this->data.emplace (data_id, data_ptr);
		assert (report.second); // already in map ??? wuut?

		EntityDataCollection& collection = this->entity_data.at (entity_id);
		collection.emplace (data_id);

		if (0 == this->type_lookup.count (type_name)) {
			this->type_lookup.emplace (type_name, EntityDataCollection ());
		}
		this->type_lookup.at (type_name).emplace (data_id);

		return dynamic_cast<TDataType*> (data_ptr);
	}

	EntityCollection
	get_all_entities () {
		EntityCollection collection;

		for (auto &entry : this->entity_data) {
			collection.emplace (entry.first);
		}

		return collection;
	}

	template<class TDataType> EntityCollection
	get_entities_with () {
		static_assert (
			all_derived_from_data<TDataType>::value,
			"Given type has to be derived from Data<>!"
		);

		const auto &type_name = typeid (TDataType).name ();
		assert (1 == this->data_owner.count (type_name));

		return this->data_owner.at (type_name);
	}

	template<class TArg>
	std::string get_type_name (TArg arg) {
		return typeid (TArg).name ();
	}

	template<class... TDataType> EntityCollection
	get_entities_with_all () {
		static_assert (
			all_derived_from_data<TDataType...>::value,
			"All types must be derived from Data<>"
		);

		std::unordered_set<std::type_index> types {
			std::type_index (typeid (TDataType))...
		};

		std::unordered_set<EntityID> ids;
		for (auto &type : types) {
			for (auto &id : this->data_owner.at (type.name ())) {
				ids.emplace (id);
			}
		}

		return EntityCollection (ids.begin (), ids.end ());
	}

};

}

#endif
