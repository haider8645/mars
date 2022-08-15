/*
 *  Copyright 2022, DFKI GmbH Robotics Innovation Center
 *
 *  This file is part of the MARS simulation framework.
 *
 *  MARS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  MARS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with MARS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CoreODEObjects.h"
#include <mars/interfaces/terrainStruct.h>

namespace mars {
namespace sim {

  using namespace utils;
  using namespace interfaces;

  ODEBox::ODEBox(std::shared_ptr<PhysicsInterface> world, NodeData * nodeData) : ODEObject(world, nodeData) {
    // At this moment we have not much things to do here. ^_^
    std::cout << "DEBUGGG: in ODEBox Constructor " << __FILE__ << ":" << __LINE__ << std::endl;
    createNode(nodeData); // pass a function pointer?    
  }

  ODEBox::~ODEBox(void) {
    std::cout << "DEBUGGG: in ODEBox Destructor " << __FILE__ << ":" << __LINE__ << std::endl;
  }

  ODEObject* ODEBox::clone() const{
    return new ODEBox(*this);
  }  

  bool ODEBox::createODEGeometry(interfaces::NodeData *node){
    if (!node->inertia_set && 
        (node->ext.x() <= 0 || node->ext.y() <= 0 || node->ext.z() <= 0)) {
      LOG_ERROR("Cannot create Node \"%s\" (id=%lu):\n"
                "  Box Nodes must have ext.x(), ext.y(), and ext.z() > 0.\n"
                "  Current values are: x=%g; y=%g, z=%g",
                node->name.c_str(), node->index,
                node->ext.x(), node->ext.y(), node->ext.z());
      return false;
    }

    // build the ode representation
    nGeom = dCreateBox(theWorld->getSpace(), (dReal)(node->ext.x()),
                        (dReal)(node->ext.y()), (dReal)(node->ext.z()));

    // create the mass object for the box
    if(node->inertia_set) {
      setInertiaMass(node);
    }
    else if(node->density > 0) {
      dMassSetBox(&nMass, (dReal)(node->density), (dReal)(node->ext.x()),
                  (dReal)(node->ext.y()),(dReal)(node->ext.z()));
    }
    else if(node->mass > 0) {
      dReal tempMass =(dReal)(node->mass);
      dMassSetBoxTotal(&nMass, tempMass, (dReal)(node->ext.x()),
                        (dReal)(node->ext.y()),(dReal)(node->ext.z()));
    }

    std::cout << "Created Box!" << std::endl;
    return true;
  }

  ODECapsule::ODECapsule(std::shared_ptr<PhysicsInterface> world, NodeData * nodeData) : ODEObject(world, nodeData) {
    // At this moment we have not much things to do here. ^_^
    std::cout << "DEBUGGG: in ODECapsule Constructor " << __FILE__ << ":" << __LINE__ << std::endl;
    createNode(nodeData); // pass a function pointer?
  }

  ODECapsule::~ODECapsule(void) {
    std::cout << "DEBUGGG: in ODECapsule Destructor " << __FILE__ << ":" << __LINE__ << std::endl;
  }

  ODEObject* ODECapsule::clone() const{
    return new ODECapsule(*this);
  }  
  
  bool ODECapsule::createODEGeometry(interfaces::NodeData *node){
    if (!node->inertia_set && (node->ext.x() <= 0 || node->ext.y() <= 0)) {
      LOG_ERROR("Cannot create Node \"%s\" (id=%lu):\n"
                "  Capsule Nodes must have ext.x() and ext.y() > 0.\n"
                "  Current values are: x=%g; y=%g",
                node->name.c_str(), node->index,
                node->ext.x(), node->ext.y());
      return false;
    }

    // build the ode representation
    nGeom = dCreateCapsule(theWorld->getSpace(), (dReal)node->ext.x(),
                            (dReal)node->ext.y());

    // create the mass object for the capsule
    if(node->inertia_set) {
      setInertiaMass(node);
    }
    else if(node->density > 0) {
      dMassSetCapsule(&nMass, (dReal)node->density, 3, (dReal)node->ext.x(),
                      (dReal)node->ext.y());
    }
    else if(node->mass > 0) {
      dMassSetCapsuleTotal(&nMass, (dReal)node->mass, 3, (dReal)node->ext.x(),
                            (dReal)node->ext.y());
    }
    std::cout << "Created ODECapsule" << std::endl;
    return true;
  }

  ODECylinder::ODECylinder(std::shared_ptr<PhysicsInterface> world, NodeData * nodeData) : ODEObject(world, nodeData) {
    // At this moment we have not much things to do here. ^_^
    std::cout << "DEBUGGG: in ODECylinder Constructor " << __FILE__ << ":" << __LINE__ << std::endl;
    createNode(nodeData); // pass a function pointer?
  }

  ODECylinder::~ODECylinder(void) {
    std::cout << "DEBUGGG: in ODECylinder Destructor " << __FILE__ << ":" << __LINE__ << std::endl;
  }

  ODEObject* ODECylinder::clone() const{
    return new ODECylinder(*this);
  }  

  bool ODECylinder::createODEGeometry(interfaces::NodeData *node){
    if (!node->inertia_set && (node->ext.x() <= 0 || node->ext.y() <= 0)) {
      LOG_ERROR("Cannot create Node \"%s\" (id=%lu):\n"
                "  Cylinder Nodes must have ext.x() and ext.y() > 0.\n"
                "  Current values are: x=%g; y=%g",
                node->name.c_str(), node->index,
                node->ext.x(), node->ext.y());
      return false;
    }

    // build the ode representation
    nGeom = dCreateCylinder(theWorld->getSpace(), (dReal)node->ext.x(),
                            (dReal)node->ext.y());

    // create the mass object for the cylinder
    if(node->inertia_set) {
      setInertiaMass(node);
    }
    else if(node->density > 0) {
      dMassSetCylinder(&nMass, (dReal)node->density, 3, (dReal)node->ext.x(),
                        (dReal)node->ext.y());
    }
    else if(node->mass > 0) {
      dMassSetCylinderTotal(&nMass, (dReal)node->mass, 3, (dReal)node->ext.x(),
                            (dReal)node->ext.y());
    }
    std::cout << "Created ODECylinder!" << std::endl;
    return true;
  }

  ODEHeightField::ODEHeightField(std::shared_ptr<PhysicsInterface> world, NodeData * nodeData) : ODEObject(world, nodeData) {
    // At this moment we have not much things to do here. ^_^
    std::cout << "DEBUGGG: in ODEHeightField Constructor " << __FILE__ << ":" << __LINE__ << std::endl;
    height_data = 0;
    createNode(nodeData); // pass a function pointer?
  }

  ODEHeightField::~ODEHeightField(void) {
    std::cout << "DEBUGGG: in ODEHeightField Destructor " << __FILE__ << ":" << __LINE__ << std::endl;
    if(height_data) free(height_data);
    if(terrain) free (terrain);
  }

  ODEHeightField::ODEHeightField(const ODEHeightField& rhs)
  : ODEObject(rhs) {
    terrain = new interfaces::terrainStruct(*(rhs.terrain));
    height_data = new dReal(*(rhs.height_data));    
  }

  ODEObject* ODEHeightField::clone() const{
    return new ODEHeightField(*this);
  }  

  dReal heightfield_callback(void* pUserData, int x, int z ) {
    return ((ODEHeightField*)pUserData)->heightCallback(x, z);
  }

  void ODEHeightField::destroyNode(void) {
    ODEObject::destroyNode();
    height_data = 0;
  }

  dReal ODEHeightField::heightCallback(int x, int y) {

    return (dReal)height_data[(y*terrain->width)+x]*terrain->scale;
  }

  bool ODEHeightField::createODEGeometry(interfaces::NodeData *node){
    dMatrix3 R;
    unsigned long size;
    int x, y;
    terrain = node->terrain;
    size = terrain->width*terrain->height;
    if(!height_data) height_data = (dReal*)calloc(size, sizeof(dReal));
    for(x=0; x<terrain->height; x++) {
      for(y=0; y<terrain->width; y++) {
        height_data[(terrain->height-(x+1))*terrain->width+y] = (dReal)terrain->pixelData[x*terrain->width+y];
      }
    }
    // build the ode representation
    dHeightfieldDataID heightid = dGeomHeightfieldDataCreate();

    // Create an finite heightfield.
    dGeomHeightfieldDataBuildCallback(heightid, this, heightfield_callback,
                                      terrain->targetWidth,
                                      terrain->targetHeight,
                                      terrain->width, terrain->height,
                                      REAL(1.0), REAL( 0.0 ),
                                      REAL(1.0), 0);
    // Give some very bounds which, while conservative,
    // makes AABB computation more accurate than +/-INF.
    dGeomHeightfieldDataSetBounds(heightid, REAL(-terrain->scale*2.0),
                                  REAL(terrain->scale*2.0));
    //dGeomHeightfieldDataSetBounds(heightid, -terrain->scale, terrain->scale);
    nGeom = dCreateHeightfield(theWorld->getSpace(), heightid, 1);
    dRSetIdentity(R);
    dRFromAxisAndAngle(R, 1, 0, 0, M_PI/2);
    dGeomSetRotation(nGeom, R);
    std::cout << "Created ODEHeightField!" << std::endl;
    return true;
  }

  ODEMesh::ODEMesh(std::shared_ptr<PhysicsInterface> world, NodeData * nodeData) : ODEObject(world, nodeData) {
    // At this moment we have not much things to do here. ^_^
    std::cout << "DEBUGGG: in ODEMesh Constructor " << __FILE__ << ":" << __LINE__ << std::endl;
    createNode(nodeData);
  }

  ODEMesh::~ODEMesh(void) {
    std::cout << "DEBUGGG: in ODEMesh Destructor " << __FILE__ << ":" << __LINE__ << std::endl;
    if(myVertices) free(myVertices);
    if(myIndices) free(myIndices);
    if(myTriMeshData) dGeomTriMeshDataDestroy(myTriMeshData);
  }

  ODEMesh::ODEMesh(const ODEMesh& rhs)
  : ODEObject(rhs) {
    
    //Coyping from the clone can be very problmmatic because there are alot of pointers and a deep copy will only be possible
    //if all of them are re-initialized and reassigned

  }

  ODEObject* ODEMesh::clone() const{
    return new ODEMesh(*this);
  }  

  void ODEMesh::destroyNode(void) {
    ODEObject::destroyNode();
    if(myVertices) free(myVertices);
    if(myIndices) free(myIndices);
    if(myTriMeshData) dGeomTriMeshDataDestroy(myTriMeshData);      
    myVertices = 0;
    myIndices = 0;
    myTriMeshData = 0;
  }    

  /**
   * The method creates an ode box representation of the given node.
   *
   */
  bool ODEMesh::createODEGeometry(NodeData* node) {
    int i;

    if (!node->inertia_set && 
        (node->ext.x() <= 0 || node->ext.y() <= 0 || node->ext.z() <= 0)) {
      LOG_ERROR("Cannot create Node \"%s\" (id=%lu):\n"
                "  Mesh Nodes must have ext.x(), ext.y(), and ext.z() > 0.\n"
                "  Current values are: x=%g; y=%g, z=%g",
                node->name.c_str(), node->index,
                node->ext.x(), node->ext.y(), node->ext.z());
      return false;
    }

    myVertices = (dVector3*)calloc(node->mesh.vertexcount, sizeof(dVector3));
    myIndices = (dTriIndex*)calloc(node->mesh.indexcount, sizeof(dTriIndex));
    //LOG_DEBUG("%d %d", node->mesh.vertexcount, node->mesh.indexcount);
    // first we have to copy the mesh data to prevent errors in case
    // of double to float conversion
    dReal minx, miny, minz, maxx, maxy, maxz;
    for(i=0; i<node->mesh.vertexcount; i++) {
      myVertices[i][0] = (dReal)node->mesh.vertices[i][0];
      myVertices[i][1] = (dReal)node->mesh.vertices[i][1];
      myVertices[i][2] = (dReal)node->mesh.vertices[i][2];
      if(i==0) {
        minx = myVertices[i][0];
        maxx = myVertices[i][0];
        miny = myVertices[i][1];
        maxy = myVertices[i][1];
        minz = myVertices[i][2];
        maxz = myVertices[i][2];
      }
      else {
        if(minx > myVertices[i][0]) minx = myVertices[i][0];
        if(maxx < myVertices[i][0]) maxx = myVertices[i][0];
        if(miny > myVertices[i][1]) miny = myVertices[i][1];
        if(maxy < myVertices[i][1]) maxy = myVertices[i][1];
        if(minz > myVertices[i][2]) minz = myVertices[i][2];
        if(maxz < myVertices[i][2]) maxz = myVertices[i][2];
      }
    }
    // rescale
    dReal sx = node->ext.x()/(maxx-minx);
    dReal sy = node->ext.y()/(maxy-miny);
    dReal sz = node->ext.z()/(maxz-minz);
    for(i=0; i<node->mesh.vertexcount; i++) {
      myVertices[i][0] *= sx;
      myVertices[i][1] *= sy;
      myVertices[i][2] *= sz;
    }
    for(i=0; i<node->mesh.indexcount; i++) {
      myIndices[i] = (dTriIndex)node->mesh.indices[i];
    }

    // then we can build the ode representation
    myTriMeshData = dGeomTriMeshDataCreate();
    dGeomTriMeshDataBuildSimple(myTriMeshData, (dReal*)myVertices,
                                node->mesh.vertexcount,
                                myIndices, node->mesh.indexcount);
    nGeom = dCreateTriMesh(theWorld->getSpace(), myTriMeshData, 0, 0, 0);

    // at this moment we set the mass properties as the mass of the
    // bounding box if no mass and inertia is set by the user
    if(node->inertia_set) {
      setInertiaMass(node);
    }
    else if(node->density > 0) {
      dMassSetBox(&nMass, (dReal)node->density, (dReal)node->ext.x(),
                  (dReal)node->ext.y(),(dReal)node->ext.z());
    }
    else if(node->mass > 0) {
      dMassSetBoxTotal(&nMass, (dReal)node->mass, (dReal)node->ext.x(),
                        (dReal)node->ext.y(),(dReal)node->ext.z());
    }
    std::cout << "Created ODEMesh" << std::endl;
    return true;
  }

  ODEPlane::ODEPlane(std::shared_ptr<PhysicsInterface> world, NodeData * nodeData) : ODEObject(world, nodeData) {
    // At this moment we have not much things to do here. ^_^
    std::cout << "DEBUGGG: in ODEPlane Constructor " << __FILE__ << ":" << __LINE__ << std::endl;
    createNode(nodeData); // pass a function pointer?
  }

  ODEPlane::~ODEPlane(void) {
    std::cout << "DEBUGGG: in ODEPlane Destructor " << __FILE__ << ":" << __LINE__ << std::endl;
  }

  ODEObject* ODEPlane::clone() const{
    return new ODEPlane(*this);
  }    

  bool ODEPlane::createODEGeometry(interfaces::NodeData *node){
    // build the ode representation
    nGeom = dCreatePlane(theWorld->getSpace(), 0, 0, 1, (dReal)node->pos.z());
    std::cout << "Created Plane!" << std::endl;
    return true;
  }

  ODESphere::ODESphere(std::shared_ptr<PhysicsInterface> world, NodeData * nodeData) : ODEObject(world, nodeData) {
    // At this moment we have not much things to do here. ^_^
    std::cout << "DEBUGGG: in ODESphere Constructor " << __FILE__ << ":" << __LINE__ << std::endl;
    createNode(nodeData);
  }

  ODESphere::~ODESphere(void) {
    std::cout << "DEBUGGG: in ODESphere Destructor " << __FILE__ << ":" << __LINE__ << std::endl;
  }

  ODEObject* ODESphere::clone() const{
    return new ODESphere(*this);
  }  

  /**
   * The method creates an ode shpere representation of the given node.
   *
   */
  bool ODESphere::createODEGeometry(NodeData* node) {
    if (!node->inertia_set && node->ext.x() <= 0) {
      LOG_ERROR("Cannot create Node \"%s\" (id=%lu):\n"
                "  Sphere Nodes must have ext.x() > 0.\n"
                "  Current value is: x=%g",
                node->name.c_str(), node->index, node->ext.x());
      return false;
    }

    // build the ode representation
    nGeom = dCreateSphere(theWorld->getSpace(), (dReal)node->ext.x());

    // create the mass object for the sphere
    if(node->inertia_set) {
      setInertiaMass(node);
    }
    else if(node->density > 0) {
      dMassSetSphere(&nMass, (dReal)node->density, (dReal)node->ext.x());
    }
    else if(node->mass > 0) {
      dMassSetSphereTotal(&nMass, (dReal)node->mass, (dReal)node->ext.x());
    }
    std::cout <<"Created ODESphere!" << std::endl;
    return true;
  }

} // end of namespace sim
} // end of namespace mars
