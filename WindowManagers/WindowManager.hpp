/**
 * @class	WindowManager
 * @brief	Interface for window manager management
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __WINDOWMANAGER_HPP__
#define __WINDOWMANAGER_HPP__

#include <string>
#include <stdint.h>
#include "Renderer.hpp"

class WindowManager
{
	public:
		/**
		 * Supported window managers
		 */
		enum WindowManagerType {
			WINDOW_MANAGER_GLUT
		};

		/**
		 * Window Manager factory
		 *
		 * @param	type	Type of window manager to be created
		 *
		 * @return Pointer to a the requested window manager or NULL
		 */
		static WindowManager *NewWindowManager(WindowManagerType type);

		/**
		 * Window Manager disposal
		 *
		 * @param	wmanager	Window manager to be disposed
		 */
		static void DeleteWindowManager(WindowManager *wmanager);

		/**
		 * Initializes the window manager
		 * @details	Prepares the window manager to be used. After calling
		 *          this method the rest of the methods can be called
		 *
		 * @return	true or false
		 */
		virtual bool init(void) = 0;

		/**
		 * Creates a new window
		 *
		 * @param	name	Title of the window
		 * @param	width	Width of the window
		 * @param	height	Height of the window
		 *
		 * @return  true or false
		 */
		virtual bool createWindow(std::string &name, uint16_t width, uint16_t height) = 0;

		/**
		 * Sets a new renderer to handle display requests
		 *
		 * @param	renderer	Renderer that will handle display/reshape requests
		 *
		 * @return	true or false
		 */
		virtual bool setRenderer(Renderer *renderer) = 0;

		/**
		 * Enters window manager main loop
		 */
		virtual void loop(void) = 0;
};

#endif
