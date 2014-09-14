/*
 * Copyright (C) 2014 Walkman
 * Author: Alessio Rocchi
 * email:  alessio.rocchi@iit.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU Lesser General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

#ifndef __TASKS_AGGREGATED_H__
#define __TASKS_AGGREGATED_H__

#include <wb_sot/Task.h>

#include <yarp/sig/all.h>
#include <list>


 namespace wb_sot {
    namespace tasks {

        class Aggregated: public Task<yarp::sig::Matrix, yarp::sig::Vector> {

        private:

            std::list< TaskType* > _tasks;
            unsigned int _aggregationPolicy;

        public:
            /**
             * @brief Aggregated
             * @param bounds a std::list of Tasks
             */
            Aggregated(const std::list<TaskType *> &tasks,
                       const unsigned int x_size);
            ~Aggregated();

            void update(const yarp::sig::Vector &x);
        };

    }
 }

#endif
