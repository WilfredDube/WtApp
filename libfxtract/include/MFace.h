  #ifndef MFACE_H
  #define MFACE_H

  #include <TopoDS_Face.hxx>
  #include <vector>
  #include "ModelUtils.h"
  #include "ModelEdge.h"

  #include <cstddef> // NULL
  #include <iomanip>
  #include <iostream>
  #include <fstream>
  #include <string>

  #include <boost/archive/tmpdir.hpp>

  #include <boost/archive/text_iarchive.hpp>
  #include <boost/archive/text_oarchive.hpp>

  #include <boost/serialization/base_object.hpp>
  #include <boost/serialization/utility.hpp>
  #include <boost/serialization/map.hpp>
  #include <boost/serialization/assume_abstract.hpp>

  namespace Fxt {

    class MFace
    {
      friend std::ostream & operator<<(std::ostream &os, const MFace &mf);
      friend class boost::serialization::access; 

      FaceID mFaceID; /*!< an integer value  represents the Face Identification number */
      PlaneType mPlaneType;
      FaceType mFaceType;
      gp_Dir mFaceNormal; //!< the normal vector to the face.
      gp_Dir mFaceUnitNormal; //!< the unit normal vector to the face.

      std::vector<ModelEdge> mFaceEdges; //!< edges of the face.

      TopoDS_Face mModelFace;

      template<class Archive>
      void serialize(Archive & ar, const unsigned int /* file_version */){
          ar & mFaceID;
      }
      
    public:

      MFace(){}

      void setTModelFace(TopoDS_Face tface);
      TopoDS_Face getTModelFace();

      void setFaceId(FaceID faceID);
      FaceID getFaceId() const;

      void setUnitNormal(gp_Dir unit_normal);
      gp_Dir getUnitNormal();

      gp_Dir getFaceNormal() const;
      void computeFaceNormal();

      void setFaceType(FaceType ftype);
      FaceType getFaceType();

      void setPlaneType(PlaneType ptype);
      PlaneType getPlaneType();

      void extractEdges();

      void setFaceEdgePosition();

      void addEdge(const ModelEdge& edge);

      std::vector<ModelEdge>& getFaceEdges();

      ModelEdge getFaceEdgesAt(int index) const;

      void getFaceEdgePosition() const;      
    };

  }

  #endif // MFACE_H
