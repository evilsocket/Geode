#include "Geode.h"

using namespace GEngine;


	
class EventHandler : public GEventHandler {
public :
	
	G3DObject O3D;
	
	EventHandler(){
		O3D.add_primitive( 400, 200, 400, 400, 600, 400, 400, 600, 800 );
		O3D.add_primitive( 400, 200, 400, 400, 200, 800, 400, 600, 800 );
		O3D.add_primitive( 800, 200, 400, 800, 600, 400, 800, 600, 800 );
		O3D.add_primitive( 800, 200, 400, 800, 200, 800, 800, 600, 800 );
		O3D.add_primitive( 400, 200, 400, 400, 600, 400, 800, 200, 400 );
		O3D.add_primitive( 800, 200, 400, 400, 600, 400, 800, 600, 400 );
		O3D.add_primitive( 400, 200, 800, 400, 600, 800, 800, 200, 800 );
		O3D.add_primitive( 800, 200, 800, 400, 600, 800, 800, 600, 800 );
		O3D.add_primitive( 400, 200, 400, 400, 200, 800, 800, 200, 400 );
		O3D.add_primitive( 800, 200, 800, 400, 200, 800, 800, 200, 400 );
		O3D.add_primitive( 400, 600, 400, 400, 600, 800, 800, 600, 400 );
		O3D.add_primitive( 800, 600, 800, 400, 600, 800, 800, 600, 400 );
	}
	
	void OnPaint(){
		//printf( "OnPaint\n" );	
	}
	void OnClose(){
		//printf( "OnClose\n" );	
	}
	virtual void OnMouseMove( unsigned int x, unsigned int y ){
		//printf( "OnMouseMove( %d, %d )\n", x, y );
	}
	void OnMouseDown( unsigned int button, unsigned int x, unsigned int y ){
		//printf( "OnMouseDown( %d, %d, %d )\n", button, x, y );
	}
	void OnMouseUp( unsigned int button, unsigned int x, unsigned int y ){
		//printf( "OnMouseUp( %d, %d, %d )\n", button, x, y );
	}
	void OnInitialize(){
		//printf( "OnInitialize\n" );
	}
	void OnKeyDown( unsigned int key, unsigned int mask ){
		//printf( "OnKeyDown( %d, %d )\n", key, mask );
		
		switch(key){
			case 27  : GCC->render(O3D).update(); break;
			case 54  : GCC->clear().update();     break;
			
			case 111 : O3D.move_camera_z( +15 ); GCC->clear().render(O3D).update(); break;
			case 114 : O3D.move_camera_x( +15 ); GCC->clear().render(O3D).update(); break;
			case 116 : O3D.move_camera_z( -15 ); GCC->clear().render(O3D).update(); break;
			case 113 : O3D.move_camera_x( -15 ); GCC->clear().render(O3D).update(); break;
		}
	}	
	void OnKeyUp( unsigned int key, unsigned int mask ){
		//printf( "OnKeyUp( %d, %d )\n", key, mask );
	}
};

int main(){
	
	EventHandler e;
	GWindow w( "prova", 1280, 1024, &e );
	
	while(1){ sleep(1); }
	
	return 0;	
}
