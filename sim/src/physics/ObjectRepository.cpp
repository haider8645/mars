// Copyright 2016 Adam B. Singer
// Contact: PracticalDesignBook@gmail.com
//
// This file is part of pdCalc.
//
// pdCalc is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// pdCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pdCalc; if not, see <http://www.gnu.org/licenses/>.

#include "ObjectRepository.h"
#include "ODEObject.h"
#include <unordered_map>
//#include "../utilities/Exception.h"
#include <sstream>

using std::string;
using std::unordered_map;
using std::set;

namespace mars {
namespace sim {    

class ObjectRepository::ObjectRepositoryImpl
{
public:
    ObjectRepositoryImpl();
    void registerObject(const string& name, ODEObjectPtr c);
    ODEObjectPtr deregisterObject(const string& name);

    size_t getNumberObjects() const { return repository_.size(); }
    ODEObjectPtr allocateObject(const string& name) const;

    bool hasKey(const string& s) const;
    set<string> getAllObjectNames() const;

    void printHelp(const std::string& object, std::ostream& os);

    void clearAllObjects();

private:
    using Repository = unordered_map<string, ODEObjectPtr>;
    Repository repository_;
};

ObjectRepository::ObjectRepositoryImpl::ObjectRepositoryImpl()
{
}

bool ObjectRepository::ObjectRepositoryImpl::hasKey(const string& s) const
{
    return repository_.find(s) != repository_.end();
}

set<string> ObjectRepository::ObjectRepositoryImpl::getAllObjectNames() const
{
    set<string> tmp;

    for(auto i = repository_.begin(); i != repository_.end(); ++i)
        tmp.insert(i->first);

    return tmp;
}

void ObjectRepository::ObjectRepositoryImpl::printHelp(const std::string& Object, std::ostream& os)
{
    /*
    auto it = repository_.find(Object);
    if(it != repository_.end())
        os << Object << ": " << it->second->helpMessage();
    else
        os << Object << ": no help entry found";

    return;
    */
}

void ObjectRepository::ObjectRepositoryImpl::clearAllObjects()
{
    repository_.clear();
    return;
}

void ObjectRepository::ObjectRepositoryImpl::registerObject(const string& name, ODEObjectPtr object)
{
    if( hasKey(name) )
    {
        std::ostringstream oss;
        oss << "Object " << name << " already registered";
        //throw Exception{ oss.str() };
    }
    else
        repository_.emplace( name, std::move(object) );
    return;
}

ODEObjectPtr ObjectRepository::ObjectRepositoryImpl::deregisterObject(const string& name)
{
    if( hasKey(name) )
    {
        auto i = repository_.find(name);
        auto tmp = ODEObjectPtr(i->second.release());
        repository_.erase(i);
        return tmp;
    }
    else return ODEObjectPtr(nullptr);
}

ODEObjectPtr ObjectRepository::ObjectRepositoryImpl::allocateObject(const string &name) const
{
    if( hasKey(name) )
    {
        const auto& object = repository_.find(name)->second;
        auto tmp = ODEObjectPtr(object->clone());
        std::cout << "Cloned! " << std::endl;
        return tmp ;

    }
    else return ODEObjectPtr(nullptr);
}

ObjectRepository::ObjectRepository()
: pimpl_{ new ObjectRepositoryImpl }
{
}

ObjectRepository::~ObjectRepository()
{ }

ObjectRepository& ObjectRepository::Instance()
{
    static ObjectRepository instance;
    return instance;
}

void ObjectRepository::registerObject(const string& name, ODEObjectPtr c)
{
    pimpl_->registerObject( name, std::move(c) );
    return;
}
ODEObjectPtr ObjectRepository::deregisterObject(const string& name)
{
    return pimpl_->deregisterObject(name);
}

size_t ObjectRepository::getNumberObjects() const
{
    return pimpl_->getNumberObjects();
}

ODEObjectPtr ObjectRepository::allocateObject(const string& name) const
{
    return pimpl_->allocateObject(name);
}

bool ObjectRepository::hasKey(const string& s) const
{
    return pimpl_->hasKey(s);
}

set<string> ObjectRepository::getAllObjectNames() const
{
    return pimpl_->getAllObjectNames();
}

void ObjectRepository::printHelp(const std::string& name, std::ostream& os) const
{
    pimpl_->printHelp(name, os);
    return;
}

void ObjectRepository::clearAllObjects()
{
    pimpl_->clearAllObjects();
    return;
}

}
}