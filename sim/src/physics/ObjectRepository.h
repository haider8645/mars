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

#ifndef OBJECT_REPOSITORY_H
#define OBJECT_REPOSITORY_H

// The CommandRepository class is responsible for returning a Command by name. New commands
// can be dynamically added at runtime (to support) plugins, and commands can also be
// deregistered (if desired if a plugin is removed). New commands are returned as clones
// of the registered Command. This makes use of the Prototype pattern.

#include <memory>
#include <string>
#include <set>
#include <iostream>
#include "ODEObject.h"

namespace mars {
namespace sim {    

class ObjectRepository
{
    class ObjectRepositoryImpl;
public:
    static ObjectRepository& Instance();

    // register a new command for the factory: throws if a command with the
    // same name already exists...deregister first to replace a command
    void registerObject(const std::string& name, ODEObjectPtr c);

    // deregister a command: returns the pointer to a command and subsequently
    // removes it from the internal database of commands...returns a nullptr
    // if the Object does not exist
    ODEObjectPtr deregisterObject(const std::string& name);

    // returns the number of Objects currently registered
    size_t getNumberObjects() const;

    // returns a pointer to a Object without deregistering the Object...returns
    // a nullptr if the Object does not exist
    ODEObjectPtr allocateObject(const std::string& name) const;

    // returns true if the Object is present, false otherwise
    bool hasKey(const std::string& s) const;

    // returns a set of all the Objects
    std::set<std::string> getAllObjectNames() const;

    // prints help for Object
    void printHelp(const std::string& name, std::ostream&) const;

    // clears all Objects; mainly needed for testing
    void clearAllObjects();

private:
    ObjectRepository();
    ~ObjectRepository();

    ObjectRepository(ObjectRepository&) = delete;
    ObjectRepository(ObjectRepository&&) = delete;
    ObjectRepository& operator=(ObjectRepository&) = delete;
    ObjectRepository& operator=(ObjectRepository&&) = delete;

    std::unique_ptr<ObjectRepositoryImpl> pimpl_;
};

}
}
#endif