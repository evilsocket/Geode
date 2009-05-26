#include "Geode.h"

using namespace GEngine;


	
class EventHandler : public GEventHandler {
public :

	int mx, my;
	
	
	EventHandler( ){
		mx = my = 0;	
	}
	
	void OnPaint( GFrameBuffer *buffer ){
		printf( "OnPaint\n" );	
	}
	void OnClose(){
		printf( "OnClose\n" );	
	}
	virtual void OnMouseMove( unsigned int x, unsigned int y ){
		printf( "OnMouseMove( %d, %d )\n", x, y );
		mx = x;
		my = y;
		buffer->image->f.put_pixel( buffer->image, mx, my, 0xFF );
		window->update();
	}
	void OnMouseDown( unsigned int button, unsigned int x, unsigned int y ){
		printf( "OnMouseDown( %d, %d, %d )\n", button, x, y );
	}
	void OnMouseUp( unsigned int button, unsigned int x, unsigned int y ){
		printf( "OnMouseUp( %d, %d, %d )\n", button, x, y );
	}
	void OnInitialize(){
		printf( "OnInitialize\n" );
	}
	void OnKeyDown( unsigned int key, unsigned int mask ){
		printf( "OnKeyDown( %d, %d )\n", key, mask );
	}	
	void OnKeyUp( unsigned int key, unsigned int mask ){
		printf( "OnKeyUp( %d, %d )\n", key, mask );
	}
};

int main(){
	
	EventHandler e;
	GWindow w( "prova", 1280, 1024, &e );
	
	while(1){ sleep(1); }
	
	return 0;	
}
