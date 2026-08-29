#include "ui_menu.hh"

// //extern "C" void selectView(int);           // cgx.c:4168 — unchanged



// // constexpr MenuItem kViewItems[] = {
// //   {"Show All Elements With Light", 1,  selectView},
// //   {"Show Bad Elements",            2,  selectView},
// //   {"FILL",                         3,  selectView, "F"},
// //   {"LINES",                        4,  selectView, "L"},
// //   {"DOTS",                         5,  selectView, "D"},
// //   {"Flip shell elements",         18,  selectView},
// //   {"Toggle Culling Back/Front",    6,  selectView, {}, []{ return g_view.cullBackface; }},
// //   {"Toggle Illuminate Backface",  15,  selectView, {}, []{ return g_view.illuminateResults; }},
// //   {"Toggle Model Edges",           7,  selectView, {}, []{ return g_view.modelEdges; }},
// //   {"Toggle Element Edges",         8,  selectView, {}, []{ return g_view.elementEdges; }},
// //   {"Toggle Surfaces/Volumes",      9,  selectView, {}, []{ return g_view.surfaces; }},
// //   {"Toggle Move-Z/Zoom",          10,  selectView},
// //   {"Toggle Background Color",     11,  selectView},
// //   {"Toggle Vector-Plot",          12,  selectView},
// //   {"Toggle Add-Displacement",     13,  selectView, {}, []{ return g_view.addDisplacement; }},
// //   {"Toggle Shaded Results",       14,  selectView},
// //   {"Toggle Transparency",         16,  selectView, {}, []{ return g_view.transparency; }},
// //   {"Toggle Ruler",                17,  selectView, {}, []{ return g_view.ruler; }},
// // };


// subsubmenu_animTune = glutCreateMenu( tuneAnimation );
//   glutAddMenuEntry(" RESET TO 1.", 0);
//   glutAddMenuEntry(" tune * 10", 1);
//   glutAddMenuEntry(" tune *  5", 2);
//   glutAddMenuEntry(" tune *  2", 3);
//   glutAddMenuEntry(" tune /  2", 4);
//   glutAddMenuEntry(" tune /  5", 5);
//   glutAddMenuEntry(" tune / 10", 6);

//   subsubmenu_animSteps = glutCreateMenu( stepsAnimation );
//   glutAddMenuEntry("  4 Steps", 4);
//   glutAddMenuEntry("  8 Steps", 8);
//   glutAddMenuEntry(" 12 Steps", 12);
//   glutAddMenuEntry(" 24 Steps", 24);
//   glutAddMenuEntry(" 36 Steps", 36);
//   glutAddMenuEntry(" 72 Steps", 72);

//   subsubmenu_animPeriod = glutCreateMenu( newTimePerPeriod );
//   glutAddMenuEntry(" Fastest     ", 1);
//   glutAddMenuEntry(" 1,0 seconds ", 2);
//   glutAddMenuEntry(" 1,2 seconds ", 3);
//   glutAddMenuEntry(" 1,5 seconds ", 4);
//   glutAddMenuEntry(" 2,0 seconds ", 5);
//   glutAddMenuEntry(" 5,0 seconds ", 6);

//   subsubmenu_colormap = glutCreateMenu( changeColormap );
//   for(i=0; i<cmaps; i++) glutAddMenuEntry(cmap_names[i], i+1);

//   submenu_view = glutCreateMenu( selectView );
//   glutAddMenuEntry("Show All Elements With Light", 1);
//   glutAddMenuEntry("Show Bad Elements", 2);
//   glutAddMenuEntry(" FILL  ", 3);
//   glutAddMenuEntry(" LINES ", 4);
//   glutAddMenuEntry(" DOTS ", 5);
//   glutAddMenuEntry("Flip shell elements", 18);
//   glutAddMenuEntry("Toggle Culling Back/Front", 6);
//   glutAddMenuEntry("Toggle Illuminate Backface", 15);
//   glutAddMenuEntry("Toggle Model Edges", 7);
//   glutAddMenuEntry("Toggle Element Edges", 8);
//   glutAddMenuEntry("Toggle Surfaces/Volumes", 9);
//   glutAddMenuEntry("Toggle Move-Z/Zoom", 10);
//   glutAddMenuEntry("Toggle Background Color", 11);
//   glutAddMenuEntry("Toggle Vector-Plot", 12);
//   glutAddMenuEntry("Toggle Add-Displacement", 13);
//   glutAddMenuEntry("Toggle Shaded Results", 14);
//   glutAddMenuEntry("Toggle Transparency", 16);
//   glutAddMenuEntry("Toggle Ruler", 17);
//   glutAddSubMenu  ("Colormap", subsubmenu_colormap );

//   submenu_animate = glutCreateMenu( changeAnimation );
//   glutAddMenuEntry("Start", 1);
//   glutAddSubMenu  ("Tune-Value ", subsubmenu_animTune );
//   glutAddSubMenu  ("Steps per Period", subsubmenu_animSteps );
//   glutAddSubMenu  ("Time per Period ", subsubmenu_animPeriod );
//   glutAddMenuEntry("Toggle Real Displacements", 2);
//   glutAddMenuEntry("Toggle Static Model Edges", 3);
//   glutAddMenuEntry("Toggle Static Element Edges", 4);
//   glutAddMenuEntry("Toggle Dataset Sequence", 5);

//   submenu_orientation = glutCreateMenu( orientModel );
//   glutAddMenuEntry( "+x View     ", 1);
//   glutAddMenuEntry( "-x View     ", 2);
//   glutAddMenuEntry( "+y View     ", 3);
//   glutAddMenuEntry( "-y View     ", 4);
//   glutAddMenuEntry( "+z View     ", 5);
//   glutAddMenuEntry( "-z View     ", 6);

//   submenu_hardcopy = glutCreateMenu( markHardcopy );
//   glutAddMenuEntry( "Tga-Hardcopy", 2);
//   glutAddMenuEntry( "Ps-Hardcopy ", 1);
//   glutAddMenuEntry( "Gif-Hardcopy", 4);
//   glutAddMenuEntry( "Png-Hardcopy", 5);
//   glutAddMenuEntry( "Start Recording Gif-Movie", 3);

//   submenu_cut   = glutCreateMenu( selectCutNode   );
//   glutAddMenuEntry( "switch plot", 9);
//   glutAddMenuEntry( "Node 1 ", 1);
//   glutAddMenuEntry( "Node 2 ", 2);
//   glutAddMenuEntry( "Node 3 ", 3);
//   glutAddMenuEntry( "Vector ", 5);
//   glutAddMenuEntry( "X plane ", 6);
//   glutAddMenuEntry( "Y plane ", 7);
//   glutAddMenuEntry( "Z plane ", 8);
//   glutAddMenuEntry( "Uncut   ", 4);

//   submenu_graph = glutCreateMenu( selectGraphMode );
//   glutAddMenuEntry( "Length ", 1);
//   glutAddMenuEntry( "Datasets ", 2);
//   glutAddMenuEntry( "Time ", 3);

//   submenu_help = glutCreateMenu( showHelp );
//   glutAddMenuEntry( "cgx Quick Reference", 1);
//   glutAddMenuEntry( "cgx Manual", 2);
//   glutAddMenuEntry( "ccx Manual", 3);
// #ifdef AFLIB
//   glutAddMenuEntry( "aflib Manual", 4);
// #endif
