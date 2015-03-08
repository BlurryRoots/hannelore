#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <cassert>
#include <typeinfo>
#include <typeindex>
#include <set>

#include <Guid.h>

#include <yanecos/IData.h>

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
	std::map<DataID, std::shared_ptr<IData>>
	DataMap
	;
typedef
	std::vector<DataID>
	EntityDataCollection
	;
typedef
	std::map<EntityID, EntityDataCollection>
	EntityDataMap
	;
typedef
	std::vector<EntityID>
	EntityCollection
	;
typedef
	std::map<std::string, EntityCollection>
	DataOwnerMap
	;
typedef
	std::map<DataID, std::string>
	DataTypeLookupMap
	;

class EntityManager {

private:
	GuidGenerator id_generator;

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
	: id_generator ()
	, data ()
	, data_owner ()
	, type_lookup ()
	, data_id_counter (0)
	, entity_data ()
	, entity_id_counter (0) {
	};

	virtual
	~EntityManager () {}

	EntityCollection
	get_all_entities () {
		EntityCollection collection;

		for (auto &entry : this->entity_data) {
			collection.push_back (entry.first);
		}

		return collection;
	}

	template<class TDataType> EntityCollection
	get_entities_with () {
		std::string type_name = typeid (TDataType).name ();
		assert (1 == this->data_owner.count (type_name));

		return this->data_owner.at (type_name);
	}

	template<class TArg>
	std::string get_type_name (TArg arg) {
		return typeid (TArg).name ();
	}

	template<class... TArg> EntityCollection
	get_entities_with_all () {
		std::vector<std::type_index> types {
			std::type_index (typeid (TArg))...
		};
		std::set<EntityID> ids;

		for (auto &type : types) {
			for (auto &id : this->data_owner.at (type.name ())) {
				ids.emplace (id);
			}
		}

		return EntityCollection (ids.begin (), ids.end ());
	}

	EntityID
	create_entity (void) {
		EntityID id = this->generate_entity_id ();

		this->entity_data.emplace (id, EntityDataCollection ());

		return id;
	}

	template<class TDataType> std::shared_ptr<TDataType>
	get_data (EntityID entity_id) const {
		std::string type_name = typeid (TDataType).name ();
		EntityDataCollection collection = this->entity_data.at (entity_id);
		if (0 == collection.size ()) {
			std::stringstream ss;

			ss << "Collection is zero for " << entity_id;
			throw std::runtime_error (ss.str ());
		}

		for (auto &data_id : collection) {
			std::string cur = this->data.at (data_id)->get_type ();
			if (cur == type_name) {
				return std::static_pointer_cast<TDataType> (this->data.at (data_id));
			}
		}

		throw std::runtime_error ("No data of type " + type_name);
	}

	template<class TDataType, class... TArgs> std::shared_ptr<TDataType>
	add_data (EntityID entity_id, TArgs&&... args) {
		std::string type_name = typeid (TDataType).name ();
		EntityDataCollection& collection = this->entity_data.at (entity_id);

		DataID data_id = this->generate_data_id ();
		collection.push_back (data_id);

		auto data_ptr = std::make_shared<TDataType> (args...);
		assert (0 == this->data.count (data_id));
		this->data.emplace (data_id, data_ptr);

		if (0 == this->data_owner.count (type_name)) {
			this->data_owner.emplace (type_name, EntityCollection ());
		}
		this->data_owner.at (type_name).push_back (entity_id);

		//this->type_lookup[data_id] = type_name;

		return data_ptr;
	}

};

}

#endif
