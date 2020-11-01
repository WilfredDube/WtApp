#include "../../../include/sheet-metal-component/bend/BendLine.h"
#include "../../../include/Computation.h"
#include "../../../include/sheet-metal-component/Point.h"
#include "../../../include/sheet-metal-component/edge/ModelEdge.h"

#include <gp_Pnt.hxx>

using namespace Fxt::SheetMetalComponent::Bend;
using namespace Fxt::SheetMetalComponent::Edge;

void BendLine::computeBendLine(
    const std::vector<std::shared_ptr<Fxt::SheetMetalComponent::Edge::ModelEdge>>& arcEdges)
{
    if(arcEdges.empty() || arcEdges.size() != 2)
        return; // error 
    
    std::vector<gp_Pnt> endPoints;

    endPoints.push_back(computeMidPoint(arcEdges[0]->getEdgeEndPoints()[0], 
                                arcEdges[0]->getEdgeEndPoints()[1])
                        );

    endPoints.push_back(computeMidPoint(arcEdges[1]->getEdgeEndPoints()[0], 
                                arcEdges[1]->getEdgeEndPoints()[1])
                        );

    gp_Pnt dirVertex(
      endPoints[0].X() - endPoints[1].X(),
      endPoints[0].Y() - endPoints[1].Y(),
      endPoints[0].Z() - endPoints[1].Z()
    );


    mBendLine = std::make_shared<gp_Lin>(endPoints[0], gp_Dir(dirVertex.X(), dirVertex.Y(), dirVertex.Z()));

    mBendLinePnt->X = endPoints[0].X();//bendLine_.Location().X();
    mBendLinePnt->Y = endPoints[0].Y();//bendLine_.Location().Y();
    mBendLinePnt->Z = endPoints[0].Z();//bendLine_.Location().Z();

    mBendLineDir->X = dirVertex.X();
    mBendLineDir->Y = dirVertex.Y();
    mBendLineDir->Z = dirVertex.Z();
}

void BendLine::makeBendLine()
{
    mBendLine = std::make_shared<gp_Lin>(
        gp_Pnt( mBendLinePnt->X, mBendLinePnt->Y, mBendLinePnt->Z), gp_Dir(mBendLineDir->X, mBendLineDir->Y, mBendLineDir->Z)
        );
}
        
std::shared_ptr<gp_Lin> BendLine::getBendLine() const 
{
    return mBendLine;
}