/*
 * Support.h
 *
 *  Created on: Jul 31, 2016
 *      Author: brian
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_

#include <stdlib.h>

class Support {
public:
	Support();
	virtual ~Support();

	static double getRandom() { return (double)(rand())/(double)(RAND_MAX); }
};

#endif /* SUPPORT_H_ */
