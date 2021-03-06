/*
 * pollingtunecontroller.h
 * Copyright (C) 2006  Remko Troncon
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef POLLINGTUNECONTROLLER_H
#define POLLINGTUNECONTROLLER_H

#include <QTimer>

#include "tune.h"
#include "tunecontroller.h"

class PollingTuneController : public TuneController
{
	Q_OBJECT

public:
	PollingTuneController();
	void setInterval(int interval);

protected slots:
	void check();

private:
	Tune prev_tune_;
	QTimer timer_;
};

#endif
