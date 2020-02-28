#pragma once
#include <stdarg.h>
#include "object.h"
#include "string.h"
#include "linked_array.h"
#include <stdlib.h>
#include <thread>

int THREADS = 2;

// forward declearation
class IntColumn;
class BoolColumn;
class FloatColumn;
class StringColumn;

/**************************************************************************
 * Column ::
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer
 * equality. */
class Column : public Object {
public:

	/** Type converters: Return same column under its actual type, or
	 *  nullptr if of the wrong type.  */
	virtual IntColumn* as_int()
	{
		return 0;
	}
	virtual BoolColumn*  as_bool()
	{
		return 0;
	}
	virtual FloatColumn* as_float()
	{
		return 0;
	}
	virtual StringColumn* as_string()
	{
		return 0;
	}

	/** Type appropriate push_back methods. Calling the wrong method is
	  * undefined behavior. **/
	virtual void push_back(int val);
	virtual void push_back(bool val);
	virtual void push_back(float val);
	virtual void push_back(String* val);

	/** Returns the number of elements in the column. */
	virtual size_t size()
	{
		return 0;
	}

	/** Return the type of this column as a char: 'S', 'B', 'I' and 'F'. */
	char get_type()
	{
		if (as_float())
		{
			return 'F';
		}
		if (as_int())
		{
			return 'I';
		}
		if (as_string())
		{
			return 'S';
		}
		if (as_bool())
		{
			return 'b';
		}
		else
		{
			abort();
		}
	}
};

/*************************************************************************
 * IntColumn::
 * Holds int values.
 */
class IntColumn : public Column {
public:
	// fields
	size_t len;
	linked_array *values;
	char *name;

	IntColumn()
	{
		len = 0;
		values = new linked_array();
	}

	IntColumn(int n, ...)
	{
		va_list ap;
		va_start(ap, n);
		len = va_arg(ap, int);

		for (int ii = 0; ii < len; ii++)
		{
			int *temp = new int;
			*temp = va_arg(ap, int);
			values->push_back(temp);
		}
	}

	~IntColumn()
	{
		len = 0;
		if (values)
		{
			delete(values);
		}
	}

	int get(size_t idx)
	{
		if (idx >= len)
		{
			return 0;
		}
		else
		{
			return *((int*)values->get(idx));
		}
	}
	IntColumn* as_int()
	{
		return this;
	}
	/** Set value at idx. An out of bound idx is undefined.  */
	void set(size_t idx, int val)
	{
		if (idx > len)
		{
			// undefined
		}
		if (idx == len)
		{
			len++;
			int *temp = new int;
			*temp = val;
			values->push_back(temp);
		}
		else
		{
			int *temp = new int;
			*temp = val;
			// delete return'd pointer
			int *ret = (int*)values->set(idx, temp);
			delete(ret);
		}


	}
	size_t size()
	{
		return len;
	}
};

// Other primitive column classes similar...
// Right.

/*************************************************************************
 * BoolColumn::
 * Holds boolean pointers.
 */
class BoolColumn : public Column {

public:
	// fields
	size_t len;
	linked_array *values;
	char *name;

	BoolColumn()
	{
		len = 0;
		values = new linked_array();
	}

	BoolColumn(int n, ...)
	{
		va_list ap;
		va_start(ap, n);
		len = va_arg(ap, int);

		for (int ii = 0; ii < len; ii++)
		{
			bool *temp = new bool;
			*temp = va_arg(ap, bool);
			values->push_back(temp);
		}
	}
	~BoolColumn()
	{
		len = 0;
		if (values)
		{
			delete(values);
		}
	}

	bool get(size_t idx)
	{
		if (idx >= len)
		{
			return 0;
		}
		else
		{
			return *((bool*)values->get(idx));
		}
	}

	BoolColumn* as_bool()
	{
		return this;
	}

	/** Set value at idx. An out of bound idx is undefined.  */
	void set(size_t idx, bool val)
	{
		if (idx > len)
		{
			// undefined
		}
		if (idx == len)
		{
			len++;
			bool *temp = new bool;
			*temp = val;
			values->push_back(temp);
		}
		else
		{
			bool *temp = new bool;
			*temp = val;
			bool *ret = (bool*)values->set(idx, temp);
			delete(ret);
		}


	}
	size_t size()
	{
		return len;
	}

};

/*************************************************************************
 * FloatColumn::
 * Holds float pointers.
 */
class FloatColumn : public Column {

public:
	// fields
	size_t len;
	linked_array *values;
	char *name;

	FloatColumn()
	{
		len = 0;
		values = new linked_array();
	}

	FloatColumn(int n, ...)
	{
		va_list ap;
		va_start(ap, n);
		len = va_arg(ap, int);

		for (int ii = 0; ii < len; ii++)
		{
			float *temp = new float;
			*temp = va_arg(ap, float);
			values->push_back(temp);
		}
	}
	~FloatColumn()
	{
		len = 0;
		if (values)
		{
			delete(values);
		}
	}

	float get(size_t idx)
	{
		if (idx >= len)
		{
			return 0;
		}
		else
		{
			return *((float*)values->get(idx));
		}
	}

	FloatColumn* as_float()
	{
		return this;
	}

	/** Set value at idx. An out of bound idx is undefined.  */
	void set(size_t idx, float val)
	{
		if (idx > len)
		{
			// undefined
		}
		if (idx == len)
		{
			len++;
			float *temp = new float;
			*temp = val;
			values->push_back(temp);
		}
		else
		{
			float *temp = new float;
			*temp = val;
			float *ret = (float*)values->set(idx, temp);
			delete(ret);
		}
	}
	size_t size()
	{
		return len;
	}
};

/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 */
class StringColumn : public Column {
public:
	size_t len;
	linked_array *values;
	char *name;

	StringColumn()
	{
		len = 0;
		values = new linked_array();
	}
	StringColumn(int n, ...)
	{
		va_list ap;
		va_start(ap, n);
		len = va_arg(ap, int);

		for (int ii = 0; ii < len; ii++)
		{
			String *temp = new String(va_arg(ap, char*));
			values->push_back(temp);
		}
	}
	StringColumn* as_string()
	{
		return this;
	}
	/** Returns the string at idx; undefined on invalid idx.*/
	String* get(size_t idx)
	{
		if (idx >= len)
		{
			return 0;
		}
		else
		{
			return ((String*)values->get(idx));
		}
	}
	/** Acquire ownership fo the string.  Out of bound idx is undefined. */
	void set(size_t idx, String* val)
	{
		if (idx > len)
		{
			// undefined
		}
		if (idx == len)
		{
			len++;
			values->push_back(val);
		}
		else
		{
			String *ret = (String*)values->set(idx, val);
			delete(ret);
		}
	}
	size_t size()
	{
		return len;
	}
};


/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
public:
	// an array of chars representing each col's type
	char *types;
	size_t rows;
	size_t cols;
	// an array of string*s 
	linked_array *row_names;
	linked_array *col_names;

	/** Copying constructor */
	Schema(Schema& from)
	{
		rows = from.rows;
		cols = from.cols;
		if (!from.types)
		{
			return;
		}
		types = duplicate(from.types);
		this->row_names = new linked_array(*from.row_names);
		this->col_names = new linked_array(*from.col_names);
	}

	/** Create an empty schema **/
	Schema()
	{
		rows = 0;
		cols = 0;
		types = new char[1];
		this->row_names = new linked_array();
		this->col_names = new linked_array();
	}

	/** Create a schema from a string of types. A string that contains
	  * characters other than those identifying the four type results in
	  * undefined behavior. The argument is external, a nullptr argument is
	  * undefined. **/
	Schema(const char* types)
	{
		this->types = duplicate(types);
		cols = strlen(types);
		rows = 0;
	}

	~Schema()
	{
		delete(types);
		delete(row_names);
		delete(col_names);
	}

	/** Add a column of the given type and name (can be nullptr), name
	  * is external. Names are expectd to be unique, duplicates result
	  * in undefined behavior. */
	void add_column(char typ, String* name)
	{
		cols++;
		char *temp = new char[strlen(types) + 2];
		strcpy(temp, types);
		temp[strlen(types)] = typ;
		delete(types);
		types = temp;
		col_names->push_back(name);
	}

	/** Add a row with a name (possibly nullptr), name is external.  Names are
	 *  expectd to be unique, duplicates result in undefined behavior. */
	void add_row(String* name)
	{
		rows++;
		row_names->push_back(name);
	}

	/** Return name of row at idx; nullptr indicates no name. An idx >= width
	  * is undefined. */
	String* row_name(size_t idx)
	{
		return (String*)row_names->get(idx);
	}

	/** Return name of column at idx; nullptr indicates no name given.
	  *  An idx >= width is undefined.*/
	String* col_name(size_t idx)
	{
		return (String*)col_names->get(idx);
	}

	/** Return type of column at idx. An idx >= width is undefined. */
	char col_type(size_t idx)
	{
		return types[idx];
	}

	/** Given a column name return its index, or -1. */
	int col_idx(const char* name)
	{
		for (int ii = 0; col_names->get(ii); ii++)
		{
			String *temp = (String*)col_names->get(ii);
			if (strcmp(temp->c_str(), name) == 0)
			{
				return ii;
			}
		}
		return -1;
	}

	/** Given a row name return its index, or -1. */
	int row_idx(const char* name)
	{
		for (int ii = 0; row_names->get(ii); ii++)
		{
			String *temp = (String*)row_names->get(ii);
			if (strcmp(temp->c_str(), name) == 0)
			{
				return ii;
			}
		}
		return -1;
	}

	/** The number of columns */
	size_t width()
	{
		return cols;
	}

	/** The number of rows */
	size_t length()
	{
		return rows;
	}
};

/*****************************************************************************
 * Fielder::
 * A field vistor invoked by Row.
 */
class Fielder : public Object {
public:

	Fielder();
	~Fielder();

	/** Called before visiting a row, the argument is the row offset in the
	  dataframe. */
	virtual void start(size_t r);

	/** Called for fields of the argument's type with the value of the field. */
	virtual void accept(bool b);
	virtual void accept(float f);
	virtual void accept(int i);
	virtual void accept(String* s);

	/** Called when all fields have been seen. */
	virtual void done();
};

/*************************************************************************
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
public:
	linked_array *items;
	// row number
	size_t idx;
	Schema scm;
	char *name;

	/** Build a row following a schema. */
	Row(Schema& scm)
	{
		this->scm = scm;
		idx = 0;
		items = new linked_array();
	}

	/** Setters: set the given column with the given value. Setting a column with
	  * a value of the wrong type is undefined. */
	void set(size_t col, int val)
	{
		int *ret = (int*)items->get(col);
		*ret = val;
	}
	void set(size_t col, float val)
	{
		float *ret = (float*)items->get(col);
		*ret = val;
	}
	void set(size_t col, bool val)
	{
		bool *ret = (bool*)items->get(col);
		*ret = val;
	}
	/** Acquire ownership of the string. */
	void set(size_t col, String* val)
	{

		String *ret = (String*)items->set(col, val);
		delete ret;
	}

	/** Set/get the index of this row (ie. its position in the dataframe. This is
	 *  only used for informational purposes, unused otherwise */
	void set_idx(size_t idx)
	{
		this->idx = idx;
	}
	size_t get_idx()
	{
		return this->idx;
	}

	/** Getters: get the value at the given column. If the column is not
	  * of the requested type, the result is undefined. */
	int get_int(size_t col)
	{
		return *(int*)items->get(col);
	}
	bool get_bool(size_t col) {
		return *(bool*)items->get(col);
	}
	float get_float(size_t col) {
		return *(float*)items->get(col);
	}
	String* get_string(size_t col) {
		return (String*)items->get(col);
	}

	/** Number of fields in the row. */
	size_t width()
	{
		return this->scm.rows;
	}

	/** Type of the field at the given position. An idx >= width is  undefined. */
	char col_type(size_t idx)
	{
		return this->scm.col_type(idx);
	}

	/** Given a Fielder, visit every field of this row. The first argument is
	  * index of the row in the dataframe.
	  * Calling this method before the row's fields have been set is undefined. */
	void visit(size_t idx, Fielder& f)
	{
		for (int ii = 0; ii < scm.width(); ii++)
		{
			if (scm.types[ii] == 'I')
			{
				f.accept(*(int*)items->get(ii));
				continue;
			}
			if (scm.types[ii] == 'F')
			{
				f.accept(*(float*)items->get(ii));
				continue;
			}
			if (scm.types[ii] == 'B')
			{
				f.accept(*(bool*)items->get(ii));
				continue;
			}
			if (scm.types[ii] == 'S')
			{
				f.accept((String*)items->get(ii));
				continue;
			}
		}
	}

};

/*******************************************************************************
 *  Rower::
 *  An interface for iterating through each row of a data frame. The intent
 *  is that this class should subclassed and the accept() method be given
 *  a meaningful implementation. Rowers can be cloned for parallel execution.
 */
class Rower : public Object {
public:
	Rower();
	~Rower();
	/** This method is called once per row. The row object is on loan and
		should not be retained as it is likely going to be reused in the next
		call. The return value is used in filters to indicate that a row
		should be kept. */
	virtual bool accept(Row& r);

	/** Once traversal of the data frame is complete the rowers that were
		split off will be joined.  There will be one join per split. The
		original object will be the last to be called join on. The join method
		is reponsible for cleaning up memory. */
	virtual void join_delete(Rower* other);
};

/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class DataFrame : public Object {
public:
	Schema *schm;
	linked_array *cols;
	linked_array *rows;

	/** Create a data frame with the same columns as the given df but with no rows or rownmaes */
	DataFrame(DataFrame& df)
	{
		schm = new Schema(*df.schm);
		cols = new linked_array(*df.cols);
		rows = new linked_array(*df.rows);
	}

	/** Create a data frame from a schema and columns. All columns are created
	  * empty. */
	DataFrame(Schema& schema)
	{
		schm = new Schema;
		*schm = schema;
		cols = new linked_array();
	}

	/** Returns the dataframe's schema. Modifying the schema after a dataframe
	  * has been created in undefined. */
	Schema& get_schema()
	{
		return *this->schm;
	}

	/** Adds a column this dataframe, updates the schema, the new column
	  * is external, and appears as the last column of the dataframe, the
	  * name is optional and external. A nullptr colum is undefined. */
	void add_column(Column* col, String* name)
	{
		schm->add_column(col->get_type(), name);
		cols->push_back(col);
	}
	/** Return the value at the given column and row. Accessing rows or
	 *  columns out of bounds, or request the wrong type is undefined.*/
	int get_int(size_t col, size_t row)
	{
		return ((IntColumn*)cols->get(col))->get(row);
	}
	bool get_bool(size_t col, size_t row)
	{
		return ((BoolColumn*)cols->get(col))->get(row);
	}
	float get_float(size_t col, size_t row)
	{
		return ((FloatColumn*)cols->get(col))->get(row);
	}
	String*  get_string(size_t col, size_t row)
	{
		return ((StringColumn*)cols->get(col))->get(row);
	}

	/** Return the offset of the given column name or -1 if no such col. */
	int get_col(String& col)
	{
		for (int ii = 0; schm->col_names->get(ii); ii++)
		{
			String *temp = (String*)schm->col_names->get(ii);
			if (col.equals(temp) == 0)
			{
				return ii;
			}
		}
		return -1;
	}

	/** Return the offset of the given row name or -1 if no such row. */
	int get_row(String& row)
	{
		for (int ii = 0; schm->row_names->get(ii); ii++)
		{
			String *temp = (String*)schm->row_names->get(ii);
			if (row.equals(temp) == 0)
			{
				return ii;
			}
		}
		return -1;
	}

	/** Set the value at the given column and row to the given value.
	  * If the column is not  of the right type or the indices are out of
	  * bound, the result is undefined. */
	void set(size_t col, size_t row, int val)
	{
		((IntColumn*)cols->get(col))->set(row, val);
	}
	void set(size_t col, size_t row, bool val)
	{
		((BoolColumn*)cols->get(col))->set(row, val);
	}
	void set(size_t col, size_t row, float val)
	{
		((FloatColumn*)cols->get(col))->set(row, val);
	}
	void set(size_t col, size_t row, String* val)
	{
		((StringColumn*)cols->get(col))->set(row, val);
	}

	/** Set the fields of the given row object with values from the columns at
	  * the given offset.  If the row is not form the same schema as the
	  * dataframe, results are undefined.
	  */
	void fill_row(size_t idx, Row& row)
	{
		for (int ii = 0; ii < schm->width(); ii++)
		{
			if (schm->types[ii] == 'S')
			{
				((StringColumn*)cols->get(ii))->set(idx, row.get_string(ii));
				continue;
			}
			if (schm->types[ii] == 'B')
			{
				((BoolColumn*)cols->get(ii))->set(idx, row.get_bool(ii));
				continue;
			}
			if (schm->types[ii] == 'I')
			{
				((IntColumn*)cols->get(ii))->set(idx, row.get_int(ii));
				continue;
			}
			if (schm->types[ii] == 'F')
			{
				((FloatColumn*)cols->get(ii))->set(idx, row.get_float(ii));
				continue;
			}
		}
	}

	/** Add a row at the end of this dataframe. The row is expected to have
	 *  the right schema and be filled with values, otherwise undedined.  */
	void add_row(Row& row)
	{
		schm->add_row(new String(row.name));
		rows->push_back(&row);
	}

	/** The number of rows in the dataframe. */
	size_t nrows()
	{
		return schm->width();
	}

	/** The number of columns in the dataframe.*/
	size_t ncols()
	{
		return schm->length();
	}

	/** Visit rows in order */
	void map(Rower& r)
	{
		for (int ii = 0; ii < schm->length(); ii++)
		{
			r.accept(*(Row*)rows->get(ii));
		}
	}

	/** Visits the rows in order from starting index to ending index. This
		is used in pmap's threads to delineate where each thread starts/ends. */
	void limited_map(Rower& r, size_t start_idx, size_t end_idx)
	{
		for (int ii = start_idx; ii < end_idx; ii++)
		{
			r.accept(*(Row*)rows->get(ii));
		}
	}

	/** This method clones the Rower and executes the map in parallel. Join is
	* used at the end to merge the results.
	*
	* This uses two threads.
	*/
	void pmap(Rower& r)
	{
		//Check if the dataframe is too small to multithread
		if (schm->length() < THREADS)
		{
			map(r);
		}
		else
		{
			Rower* r_clone = (Rower*)r.clone();

			if (schm->length() % 2)	//odd case
			{
				std::thread t1(limited_map, this, std::ref(r), 0, schm->length() / 2);
				std::thread t2(limited_map, this, std::ref(*r_clone), (size_t)schm->length() / 2 + 1, schm->length());
				t1.join();
				t2.join();

			}
			else						//even case
			{
				std::thread t1(limited_map, this, std::ref(r), 0, schm->length() / 2);
				std::thread t2(limited_map, this, std::ref(*r_clone), (size_t)schm->length() / 2, schm->length());
				t1.join();
				t2.join();
			}
			r.join_delete(r_clone);
		}
	}


	/** Create a new dataframe, constructed from rows for which the given Rower
	  * returned true from its accept method. */
	DataFrame* filter(Rower& r)
	{
		DataFrame *ret = new DataFrame(*this);
		delete(ret->rows);
		ret->rows = new linked_array();
		for (int ii = 0; ii < schm->length(); ii++)
		{
			if (r.accept(*(Row*)rows->get(ii)))
			{
				ret->rows->push_back(rows->get(ii));
			}
		}

		return ret;
	}

	/** Print the dataframe in SoR format to standard output. */
	void print()
	{
		for (int rr = 0; rr < schm->length(); rr++)
		{
			for (int cc = 0; cc < schm->width(); cc++)
			{
				if (schm->types[cc] == 'S')
				{
					printf("%s ", get_string(cc, rr)->c_str());
					continue;
				}
				if (schm->types[cc] == 'I')
				{
					printf("%d ", get_int(cc, rr));
					continue;
				}
				if (schm->types[cc] == 'B')
				{
					printf("%d ", get_bool(cc, rr));
					continue;
				}
				if (schm->types[cc] == 'F')
				{
					printf("%f ", get_float(cc, rr));
					continue;
				}
			}
			printf("\n");
		}
	}
};
