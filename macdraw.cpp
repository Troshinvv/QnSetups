//
// Created by mikhail on 5/6/21.
//

void macdraw(){
  gROOT->Macro( "/home/valeriy/flow_drawing_tools/example/style.cc" );
  auto leg1 = new TLegend( 0.5, 0.75, 0.9, 0.945 );
  auto leg2 = new TLegend( 0.5, 0.65, 0.9, 0.75 );
std::vector<TF1*> lines;
  auto v1_bw_x = DoubleDifferentialCorrelation( "~/correlation_out.root",
                                              {
                                                 "uQ/protons_PLAIN.fhcalS_RECENTERED.x1x1",
                                             } );
  v1_bw_x.SetSliceVariable("p_{T}", "GeV/c");
  v1_bw_x.SetMarker(kFullCircle);
  v1_bw_x.SetPalette( {kBlue, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet } );
  v1_bw_x.Select({{"McEvent._B", 4, 4, 8}});
  v1_bw_x.SetProjectionAxis({"TpcTracks._eta", 10,-1.,1.});
  v1_bw_x.SetSliceAxis({"TpcTracks._pT", {0.2,0.56,1.28,2.}});
  v1_bw_x.Calculate();



  auto v1_bw_y = DoubleDifferentialCorrelation( "~/correlation_out.root",
                                              {
                                                 "uQ/protons_PLAIN.fhcalS_RECENTERED.y1y1",
                                             } );
  v1_bw_y.SetSliceVariable("p_{T}", "GeV/c");
  v1_bw_y.SetMarker(kOpenSquare);
  v1_bw_y.SetPalette( {kBlue, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet } );
  v1_bw_y.Select({{"McEvent._B", 4, 4, 8}});
  v1_bw_y.SetProjectionAxis({"TpcTracks._eta", 10, -1., 1.});
  v1_bw_y.SetSliceAxis({"TpcTracks._pT", {0.2,0.56,1.28,2.}});
  v1_bw_y.Calculate();

  HeapPicture pic( "_corrected_y_slices_pT", {1000, 1000});

  pic.AddText({0.2, 0.9, "MPD Au+Au@7.7A GeV"}, 0.025);
  pic.AddText({0.2, 0.87, "#p;u_{1}Q_{1} "}, 0.025);
  pic.AddText({0.2, 0.84, "XX-FullCircle;YY-OpenSquare"}, 0.025);
 /* pic.AddText({0.2, 0.81, "SP: #LTu_{1}W1#GT"}, 0.025);
  pic.AddText({0.2, 0.78, "w/o occ. corr."}, 0.025); */

  for( auto obj : v1_bw_x.GetProjections() ){
	  obj->Fit( new TF1( obj->GetTitle().c_str(), "pol1" ) );
    auto fit = obj->GetFit();
    if( !fit )
      continue;
    auto p0 = fit->GetParameter(0);
    auto p1 = fit->GetParameter(1);
    auto fit0 = new TF1( std::data("p0_p1"+obj->GetTitle()), std::data(std::to_string(p0)+"+"+std::to_string(p1)+"*x"), -1., 1. );
    fit0->SetLineColor( fit->GetLineColor() );
    pic.AddFunction( fit0 );
    lines.push_back( fit0 );

   // pic.AddDrawable( obj );
    leg1->AddEntry( obj->GetPoints(), obj->GetTitle().c_str(), "P" );
  }
   leg2->AddEntry( lines.at(1), "p_{0}+p_{1}#upointx", "L");
  for( auto obj : v1_bw_y.GetProjections() ){
	  obj->Fit( new TF1( obj->GetTitle().c_str(), "pol1" ) );
    auto fit = obj->GetFit();
    if( !fit )
      continue;
    auto p0 = fit->GetParameter(0);
    auto p1 = fit->GetParameter(1);
    auto fit0 = new TF1( std::data("p0_p1"+obj->GetTitle()), std::data(std::to_string(p0)+"+"+std::to_string(p1)+"*x"), -1., 1. );
    fit0->SetLineColor( fit->GetLineColor() );
    pic.AddFunction( fit0 );
    lines.push_back( fit0 );

//    pic.AddDrawable( obj );
  }
/*  leg2->AddEntry( v1_bw_x.GetProjections().at(2)->GetPoints(), "XY", "P" );
  leg2->AddEntry( v1_bw_y.GetProjections().at(2)->GetPoints(), "YX", "P" ); */

  pic.SetAxisTitles({"pT", "u_{1}Q_{1}"});
  pic.AddLegend(leg1);
  pic.AddLegend(leg2);

  pic.SetXRange({-1, 1.});
  pic.SetYRange({-0.008, 0.008});
  pic.Draw();
  pic.Save("/home/valeriy/HistoQn/pSxxyyEtaFit","png");
}
