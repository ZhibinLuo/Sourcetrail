#ifndef _PLAYER_
#define _PLAYER_

#include "field.h"

class Player {
public:
	Player( Field::Token token, const char* name );
	virtual ~Player();

	virtual Field::Move Turn( const Field& field ) const = 0;

	const Field::Token& getToken() const;
	const char* getName() const;

protected:
	const Field::Token token_;
	const char* name_;
};

#endif // _PLAYER_
