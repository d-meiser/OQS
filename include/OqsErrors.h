/*
Copyright 2014 Dominic Meiser

This file is part of oqs.

oqs is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your
option) any later version.

oqs is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with oqs.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef OQS_ERRORS_H
#define OQS_ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif

enum OQS_STATUS {
	OQS_SUCCESS = 0,
	OQS_OUT_OF_MEMORY
};
typedef enum OQS_STATUS OQS_STATUS;

#ifdef __cplusplus
}
#endif
#endif
