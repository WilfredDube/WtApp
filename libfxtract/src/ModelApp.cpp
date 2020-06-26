#include <thread>
#include <string>
#include <future>
#include <memory>

#include "../include/ModelApp.h"

namespace Fxt {

  const Standard_Real kMachineModuleLength = 1000.0;

  ModelApp::ModelApp(){}

  static Model extractFeatures(const char *modelFile)
  {
    auto test = std::make_unique<Model>(modelFile);

    test->init();
    test->classifyFaces();
    test->computeBendAngles();
    test->printBendInfo();

    return *test;
  }
  
  Model ModelApp::run(const char *modelFile)
  {
    // Model modelFeatures;
    std::future<Model> futureData = std::async(std::launch::async, extractFeatures, modelFile);

    // auto modelFeatures = futureData.get();

    return futureData.get();
  }

  Model ModelApp::run(Model& test)
  {
    test.init();
    test.classifyFaces();
    test.computeBendAngles();
    test.printBendInfo();

    return test;
  }
}

    // std::vector<ModelTestFace> vface;
    // // std::vector<TopoDS_Face> tvfaces;
    // // // StepProcessor stepProcessor;
    // auto stepProcessor = make_unique<StepProcessor>();
    // // // IgesProcessor igesProcessor;
    // auto igesProcessor = make_unique<IgesProcessor>();
    // // // ModelReader mReader;
    // auto mReader = make_unique<ModelReader>();
    // // // Session session_(connectionPool_);
    // // // array<int, 2> v{5, 7};
    // // // auto [x, y] = v;
    // // std::string_view sv = "jhjhj"sv;
    // //
    // // //loads file MyFile.igs
    // std::string filename = modelFile;
    // FileFormat fileFormat = mReader->checkFileFormat(modelFile);
    // XSControl_Reader str = mReader->processModelFile(fileFormat, modelFile);
    // //
    // Standard_Integer nbs = str.NbShapes();
    // std::cout << "nbs : " << nbs << '\n';

    // switch (fileFormat) {
    //   case FileFormat::IGES_FORMAT:
    //   igesProcessor->extractFaces(str);
    //   vface = igesProcessor->getModelFaces();
    //   break;
    //   case FileFormat::STEP_FORMAT:
    //   stepProcessor->extractFaces(str);
    //   vface = stepProcessor->getModelFaces();
    //   break;
    //   default:
    //   std::cout << "Unknown file format" << '\n';
    // }
    //
    // ModelFace::classifyBS_BF(vface);
    // ModelFace::classifyFaces(vface);
    // ModelFace::classifyBends(vface);
    //
    // std::vector<MFace> v = test->getFaces();
    // // for (auto& a : v){
    //   std::cout << "Face iiiiid: " << v.size() << '\n';
    // }
    //
    // size_t count = vface.size();
    //
    //
    //
    // std::sort(vface.begin(), vface.end(), [ ]( const auto& lhs, const auto& rhs )
    // {
    //   return lhs.Radius < rhs.Radius;
    // });
    //
    // size_t nbends = 0;
    // for (size_t i = 0; i < count; i++) {
    //   if (vface[i].getFaceType() == FaceType::BEND_FACE) {
    //     // for (size_t j = 0; j < nbends/2; j++) {
    //     //   vface[i + j].setIsInternal(true);
    //     //   // std::cerr << j << '\n';
    //     // }
    //     // break;
    //     ++nbends;
    //   }
    // }
    //
    // // TODO : Find the longest bend length to find the number of required modules
    // Standard_Real longest = 0.0;
    // for (size_t i = 0; i < count; i++) {
    //   if (vface[i].getFaceType() == FaceType::BEND_FACE) {
    //     if (longest < roundd(vface[i].getBendLength())) {
    //       longest = roundd(vface[i].getBendLength());
    //       std::cout << "length : " << longest << '\n';
    //     }
    //   }
    // }
    //
    // std::cout << "longest length : " << longest << '\n';
    //
    // // vector<FRlnship> list;
    // dbo::Transaction transaction(session_);
    // for (size_t i = 0; i < count; i++) {
    //   if (vface[i].getFaceType() == FaceType::BEND_FACE) {
    //     // if (vface[i].bend_type == BendType::INTERNAL) {
    //     gp_Pnt firstnorm = vface[i].getNormal(vface[i].getJoiningFaceID1(), vface);
    //     gp_Pnt secondnorm = vface[i].getNormal(vface[i].getJoiningFaceID2(), vface);
    //     vface[i].setBendAngle(computeAngle(firstnorm, secondnorm));
    //     std::cout << "F"<< vface[i].getJoiningFaceID1() << "---B" << vface[i].getFaceId() << "---F" <<vface[i].getJoiningFaceID2()<< " = ";
    //
    //     // struct FRlnship rln;
    //     // rln.face_id1 = vface[i].getJoiningFaceID1();
    //     // rln.bend_id = vface[i].getFaceId();
    //     // rln.face_id2 = vface[i].getJoiningFaceID2();
    //     //
    //     // list.push_back(rln);
    //
    //     std::cout << ", Angle : " << vface[i].getBendAngle();
    //     std::cout << ", Radius : " << vface[i].getFaceRadius();
    //     // std::cout << ", Dot product: " << compute_dot_product(&gp_Pnt(0, 1, 0), &gp_Pnt(3.60355e-16, -1, 0));
    //     std::cout << ", Bend Length: " << vface[i].getBendLength()<< " mm, ";
    //     vface[i].printUnitNormal();
    //
    //     {
    //       dbo::ptr<BendFeature> BF(Wt::cpp14::make_unique<BendFeature>());
    //       // std::unique_ptr<ModelFile> MF{new ModelFile()};
    //       BendFeature *b = BF.modify();
    //
    //       b->bend_id = vface[i].getFaceId();
    //       b->face_id1 = vface[i].getJoiningFaceID1();
    //       b->face_id2 =  vface[i].getJoiningFaceID2();
    //       b->bend_angle = roundd(vface[i].getBendAngle());
    //       b->bend_radius = roundd(vface[i].getFaceRadius());
    //       b->bend_thickness = roundd(ModelFace::thickness);
    //       b->bend_length = roundd(vface[i].getBendLength());
    //       b->modelFile = p;
    //
    //       int num_of_modules = 0;
    //       if (longest > kMachineModuleLength){
    //         num_of_modules = longest / kMachineModuleLength;
    //         if ((static_cast<int>(longest) % static_cast<int>(kMachineModuleLength)) > 0) {
    //           ++num_of_modules;
    //         }
    //       } else {
    //         num_of_modules = 1;
    //       }
    //       p.modify()->nbends = nbends / 2;
    //       p.modify()->number_of_modules = num_of_modules;
    //
    //       typedef dbo::collection< dbo::ptr<Tool> > Tools;
    //       // dbo::ptr<Tool> joe = session_.query<dbo::ptr<Tool>>("select u from tools u").where("tool_angle = " + std::to_string(b->bend_angle)).bind("Joe");
    //       dbo::collection<dbo::ptr<Tool>> joe = session_.find<Tool>().where("tool_angle = ?").bind(b->bend_angle);
    //       for (Tools::const_iterator i = joe.begin(); i != joe.end(); ++i){
    //         dbo::ptr<Tool> tool = *i;
    //         std::cout << "Tool ID : " << tool->tool_id << " Angle : " << tool->tool_angle << '\n';
    //         b->bending_tool_id = tool->tool_id;
    //         break;
    //       }
    //
    //
    //       session_.add(BF);
    //       //
    //       // ModelApp::run(saveName.c_str(), p);
    //     }
    //     // }
    //   }
    // }
    // session_.flush();
    // transaction.commit();
    //
    // // findConnection(list);
    // // std::vector<int> R1;
    // // queue<FRlnship> Q;
    // // preFAG(list);
    // // // FAG(list, R1, Q, 0, list[0].face_id1);
    // //
    // // for (auto frl : R1){
    // //   std::cout << frl << " - ";
    // // }
    // //
    // // std::cout << '\n';
    //
    // std::cout << "SIZE : " <<vface.size()<< '\n';
    // // std::cout << "SIZE : " << list.size()<< '\n';
    // std::cout << "File name : " << modelFile << endl;
    // std::cout << "Thickness : " << std::setprecision(3) << ModelFace::thickness << " mm" << '\n';
    // std::cout << "# of Bends: " << ModelFace::nbends / 2 << '\n';

//     return 0;
//   }
// }
