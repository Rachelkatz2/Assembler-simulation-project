/**Import files.**/
#include "global.h"
#include "parsings.h"


/**
  This function updates the addresses of non-external and non-code labels.
  It adds the instruction counter (ei_ic) to the address of each label.
  @return void.
*/
void betweenfun()
{
	int i=0;
	for(;i<ei_index;i++)/*The function loops through each label in the symbol table and adds the instruction counter (ei_ic) to the label's address.*/
	{
		if(es_label[i].isExternal==0&& es_label[i].isCode==0)/*if the label is not an external label and not a code label. This is done to ensure that the addresses of the labels are.*/
		{
			es_label[i].address+=ei_ic;/*updated correctly after the code is generated.*/
		}
	}


}

