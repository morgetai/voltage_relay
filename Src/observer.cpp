#include "observer.h"




/* ===============================================================================
                       ##### subject member functions #####
 ===============================================================================  */
 
 /**
   * @brief attachs given observer
   * @note   
   * @param pointer to observer which will be attached
   * @retval none
   */
void Subject::attach(Observer* entry)
{
	viewers[index] = entry;
	++index;
}

/**
  * @brief detechs the given observer
  * @note   
  * @param pointer to observer which will be detached
  * @retval none
  */
void Subject::detach(Observer* entry)
{
	uint8_t i = 0;
	for (; i < index;++i )
		{
			if (viewers[i] == entry)
			{
				viewers[i] = NULL;
				break;
			}
		}
		for (; i < index;++i)
			viewers[i] = viewers[i+1];
		--index;
}

/**
  * @brief notifyes all observers
  * @note  update is a virtual function that should be implemented in all observers
  * @retval none
  */
void Subject::notify()
{
	for (uint8_t i = 0; i < index;++i)
		viewers[i]->update();
}
