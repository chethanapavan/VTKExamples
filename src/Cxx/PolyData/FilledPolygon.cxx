#include <vtkSphereSource.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkSampleFunction.h>
#include <vtkPolyData.h>
#include <vtkStripper.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkColor.h>

int main(int argc, char *argv[])
{
  // Define colors
  auto colors =
  	vtkSmartPointer<vtkNamedColors>::New();
  vtkColor3d actorColor = colors->GetColor3d("Yellow");
  vtkColor3d  EdgeColour = colors->GetColor3d("LimeGreen");
  vtkColor3d BackgroundColour = colors->GetColor3d("Black");
  
  // create a sphere
  auto sphere =
  	vtkSmartPointer<vtkSphereSource>::New();
  sphere->SetRadius(50);
  sphere->SetThetaResolution(100);
  sphere->SetPhiResolution(100);
  
  //Create a mapper and an actor
  auto mapper =
  	vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(sphere->GetOutputPort());
  
  // create a plane to cut, here it cuts in the XZ direction(xz normal = (1, 0, 0); XY = (0, 0, 1), YZ = (0, 1, 0)
  auto plane =
  	vtkSmartPointer<vtkPlane>::New();
  plane->SetOrigin(20, 0, 0);
  plane->SetNormal(1, 0, 0);
  
  // create cutter
  auto cutter =
  	vtkSmartPointer<vtkCutter>::New();
  cutter->SetCutFunction(plane);
  cutter->SetInputConnection(sphere->GetOutputPort());
  cutter->Update();
  //vtkStripper
  auto cutStrips =
  	vtkSmartPointer<vtkStripper>::New(); // Forms loops (closed polylines) from cutter
  cutStrips->SetInputConnection(cutter->GetOutputPort());
  	cutStrips->Update();
  
  auto cutPoly =
  	vtkSmartPointer<vtkPolyData>::New(); // This trick defines polygons as polyline loop
  cutPoly->SetPoints((cutStrips->GetOutput())->GetPoints());
  cutPoly->SetPolys((cutStrips->GetOutput())->GetLines());
  
  auto cutMapper =
  	vtkSmartPointer<vtkPolyDataMapper>::New();
  // cutMapper->SetInput(FeatureEdges.GetOutput())
  cutMapper->SetInputData(cutPoly);
  
  auto cutActor =
  vtkSmartPointer<vtkActor>::New();
  cutActor->GetProperty()->SetColor(actorColor.GetData());
  cutActor->GetProperty()->SetEdgeColor(EdgeColour.GetData());
  cutActor->GetProperty()->EdgeVisibilityOn();
  cutActor->GetProperty()->SetLineWidth(2);
  cutActor->GetProperty()->EdgeVisibilityOn();
  //cutActor->GetProperty()->SetOpacity(0.7);
  cutActor->SetMapper(cutMapper);
  // A renderer and render window
  auto renderer =
  	vtkSmartPointer<vtkRenderer>::New();
  //add the actors of actors of plane and Sphere
  renderer->AddActor(cutActor);
  
  renderer->SetBackground(BackgroundColour.GetData());
  renderer->GetActiveCamera()->SetPosition(223, -122, -91);
  auto renderWindow =
  	vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(600,600);
  auto renderWindowInteractor =
  	vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
  
  // Start
  renderWindow->Render();
  renderWindowInteractor->Start();
}
