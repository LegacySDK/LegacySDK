# TO FUCKING DO

looks better in the raw code but okay

<img width="1207" height="527" alt="image" src="https://github.com/user-attachments/assets/7239270c-48d7-4589-a62e-855f5f29afbc" />

## UI CONTROLLER:
```cpp
/* uicontroller shit idk */

UIController::getScreenHeight();
UIController::getScreenWidth();
// ^^ returns a float, just in case use `auto`

UIController::AnimateKeyPress();     	// pretty cool thing that must be there

UIController::FindScene();       	    // already there | EUIScene sceneType | OVERRIDE								                                            UIScene*
UIController::navigateToHomeMenu();   // already there | /* none */ | OVERRIDE									                                                  void
UIController::navigateToScene();	    // already there | int iPad, EUIScene scene, void* initData, EUILayer layer, EUIGroup group | OVERRIDE	 	  bool
UIController::navigateBack();		      // already there | int iPad, bool forceUsePad, EUIScene scene, EUILayer layer | OVERRIDE 			              bool
UIController::closeAllPlayersScenes();// already there | /* none */ | OVERRIDE 									                                                  void
UIController::closeUIScenes();	      // already there | int iPad, bool forceIpad | OVERRIDE	 							                                      void
UIController::getTopScene();	        // already there | int iPad, EUILayer layer, EUIGroup group | OVERRIDE	 					                          UIScene*

// int/floats:
UIController::UI_REPEAT_KEY_DELAY_MS;
UIController::UI_REPEAT_KEY_REPEAT_RATE_MS;
// ^ TO IMPLEMENT

```
