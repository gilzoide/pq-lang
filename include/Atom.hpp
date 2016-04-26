#pragma once

/**
 * Atom, the abstract class for PQ values
 *
 * Everything is a function in PQ, so we use this OO to help everything work
 * smooth and easily
 */
class Atom {
public:
	/**
	 * Virtual Dtor, we must have one
	 */
	virtual ~Atom ();

	/**
	 * Virtual clone method, needed for passing by value semantics
	 */
	virtual Atom *clone () = 0;

protected:
};
