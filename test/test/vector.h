#pragma once

template<class _T>
class allocator;

template<class _Ty>
class vector
	: public _Vector_alloc<!is_empty<allocator<_Ty>>::value,
	_Vec_base_types<_Ty, allocator<_Ty>> >
{	// varying size array of values
public:
	typedef vector<_Ty> _Myt;
	typedef _Vector_alloc<!is_empty<allocator<_Ty>>::value,
		_Vec_base_types<_Ty, allocator<_Ty>> > _Mybase;
	typedef allocator<_Ty> allocator_type;

	typedef typename _Mybase::_Alty _Alty;

	typedef typename _Mybase::value_type value_type;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::difference_type difference_type;
	typedef typename _Mybase::pointer pointer;
	typedef typename _Mybase::const_pointer const_pointer;
	typedef typename _Mybase::reference reference;
	typedef typename _Mybase::const_reference const_reference;

#define _VICONT(it)	it._Getcont()
#define _VIPTR(it)	(it)._Ptr

	typedef typename _Mybase::iterator iterator;
	typedef typename _Mybase::const_iterator const_iterator;

	typedef _STD reverse_iterator<iterator> reverse_iterator;
	typedef _STD reverse_iterator<const_iterator> const_reverse_iterator;

	vector()
		: _Mybase()
	{	// construct empty vector
	}

	explicit vector(const _Alloc& _Al)
		: _Mybase(_Al)
	{	// construct empty vector, allocator
	}

	explicit vector(size_type _Count)
		: _Mybase()
	{	// construct from _Count * value_type()
		if (_Buy(_Count))
		{	// nonzero, fill it
			_Alty _Alval(this->_Getal());
			_TRY_BEGIN
				_Uninitialized_default_fill_n(this->_Myfirst, _Count, _Alval);
			this->_Mylast += _Count;
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

	vector(size_type _Count, const value_type& _Val)
		: _Mybase()
	{	// construct from _Count * _Val
		_Construct_n(_Count, _STD addressof(_Val));
	}

	vector(size_type _Count, const value_type& _Val, const _Alloc& _Al)
		: _Mybase(_Al)
	{	// construct from _Count * _Val, allocator
		_Construct_n(_Count, _STD addressof(_Val));
	}

	vector(const _Myt& _Right)

		: _Mybase(_Right._Getal().select_on_container_copy_construction())


	{	// construct by copying _Right
		if (_Buy(_Right.size()))
			_TRY_BEGIN
			this->_Mylast = _Ucopy(_Right.begin(), _Right.end(),
			this->_Myfirst);
		_CATCH_ALL
			_Tidy();
		_RERAISE;
		_CATCH_END
	}

	vector(const _Myt& _Right, const _Alloc& _Al)
		: _Mybase(_Al)
	{	// construct by copying _Right, allocator
		if (_Buy(_Right.size()))
			_TRY_BEGIN
			this->_Mylast = _Ucopy(_Right.begin(), _Right.end(),
			this->_Myfirst);
		_CATCH_ALL
			_Tidy();
		_RERAISE;
		_CATCH_END
	}

	template<class _Iter,
	class = typename enable_if<_Is_iterator<_Iter>::value,
		void>::type>
		vector(_Iter _First, _Iter _Last)
		: _Mybase()
	{	// construct from [_First, _Last)
			_Construct(_First, _Last);
		}

	template<class _Iter,
	class = typename enable_if<_Is_iterator<_Iter>::value,
		void>::type>
		vector(_Iter _First, _Iter _Last, const _Alloc& _Al)
		: _Mybase(_Al)
	{	// construct from [_First, _Last) with allocator
			_Construct(_First, _Last);
		}

	template<class _Iter>
	void _Construct(_Iter _First, _Iter _Last)
	{	// initialize with [_First, _Last)
		_Construct(_First, _Last, _Iter_cat(_First));
	}

	template<class _Iter>
	void _Construct(_Iter _First, _Iter _Last, input_iterator_tag)
	{	// initialize with [_First, _Last), input iterators
		_TRY_BEGIN

		for (; _First != _Last; ++_First)
			emplace_back(*_First);

		_CATCH_ALL
			_Tidy();
		_RERAISE;
		_CATCH_END
	}

	template<class _Iter>
	void _Construct(_Iter _First, _Iter _Last, forward_iterator_tag)
	{	// initialize with [_First, _Last), forward iterators
		if (_Buy(_STD distance(_First, _Last)))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast = _Ucopy(_First, _Last, this->_Myfirst);
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

	void _Construct_n(size_type _Count, const value_type *_Pval)
	{	// construct from _Count * *_Pval
		if (_Buy(_Count))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast = _Ufill(this->_Myfirst, _Count, _Pval);
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

	vector(_Myt&& _Right)
		: _Mybase(_Right._Getal())
	{	// construct by moving _Right
		_Assign_rv(_STD forward<_Myt>(_Right), true_type());
	}

	vector(_Myt&& _Right, const _Alloc& _Al)
		: _Mybase(_Al)
	{	// construct by moving _Right, allocator
		_Assign_rv(_STD forward<_Myt>(_Right));
	}

	_Myt& operator=(_Myt&& _Right)
	{	// assign by moving _Right
		if (this != &_Right)
		{	// different, assign it
			_Tidy();
			if (_Alty::propagate_on_container_move_assignment::value
				&& this->_Getal() != _Right._Getal())
				this->_Change_alloc(_Right._Getal());

			_Assign_rv(_STD forward<_Myt>(_Right));
		}
		return (*this);
	}

	void _Assign_rv(_Myt&& _Right, true_type)
	{	// move from _Right, stealing its contents
		this->_Swap_all((_Myt&)_Right);
		this->_Myfirst = _Right._Myfirst;
		this->_Mylast = _Right._Mylast;
		this->_Myend = _Right._Myend;

		_Right._Myfirst = pointer();
		_Right._Mylast = pointer();
		_Right._Myend = pointer();
	}

	void _Assign_rv(_Myt&& _Right, false_type)
	{	// move from _Right, possibly moving its contents
		if (get_allocator() == _Right.get_allocator())
			_Assign_rv(_STD forward<_Myt>(_Right), true_type());
		else
			_Construct(_STD make_move_iterator(_Right.begin()),
			_STD make_move_iterator(_Right.end()));
	}

	void _Assign_rv(_Myt&& _Right)
	{	// assign by moving _Right
		_Assign_rv(_STD forward<_Myt>(_Right),
			typename _Alty::propagate_on_container_move_assignment());
	}


	void push_back(value_type&& _Val)
	{	// insert by moving into element at end
		if (_Inside(_STD addressof(_Val)))
		{	// push back an element
			size_type _Idx = _STD addressof(_Val) - this->_Myfirst;
			if (this->_Mylast == this->_Myend)
				_Reserve(1);
			_Orphan_range(this->_Mylast, this->_Mylast);
			this->_Getal().construct(this->_Mylast,
				_STD forward<value_type>(this->_Myfirst[_Idx]));
			++this->_Mylast;
		}
		else
		{	// push back a non-element
			if (this->_Mylast == this->_Myend)
				_Reserve(1);
			_Orphan_range(this->_Mylast, this->_Mylast);
			this->_Getal().construct(this->_Mylast,
				_STD forward<value_type>(_Val));
			++this->_Mylast;
		}
	}

	iterator insert(const_iterator _Where, _Ty&& _Val)
	{	// insert by moving _Val at _Where
		return (emplace(_Where, _STD move(_Val)));
	}

	template<class... _Valty>
	void emplace_back(_Valty&&... _Val)
	{	// insert by moving into element at end
		if (this->_Mylast == this->_Myend)
			_Reserve(1);
		_Orphan_range(this->_Mylast, this->_Mylast);
		this->_Getal().construct(this->_Mylast,
			_STD forward<_Valty>(_Val)...);
		++this->_Mylast;
	}

	template<class... _Valty>
	iterator emplace(const_iterator _Where, _Valty&&... _Val)
	{	// insert by moving _Val at _Where
		size_type _Off = _VIPTR(_Where) - this->_Myfirst;

#if _ITERATOR_DEBUG_LEVEL == 2
		if (size() < _Off)
			_DEBUG_ERROR("vector emplace iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		emplace_back(_STD forward<_Valty>(_Val)...);
		_STD rotate(begin() + _Off, end() - 1, end());
		return (begin() + _Off);
	}


	vector(_XSTD initializer_list<value_type> _Ilist,
		const _Alloc& _Al = allocator_type())
		: _Mybase(_Al)
	{	// construct from initializer_list
		insert(begin(), _Ilist.begin(), _Ilist.end());
	}

	_Myt& operator=(_XSTD initializer_list<value_type> _Ilist)
	{	// assign initializer_list
		assign(_Ilist.begin(), _Ilist.end());
		return (*this);
	}

	void assign(_XSTD initializer_list<value_type> _Ilist)
	{	// assign initializer_list
		assign(_Ilist.begin(), _Ilist.end());
	}

	iterator insert(const_iterator _Where,
		_XSTD initializer_list<value_type> _Ilist)
	{	// insert initializer_list
		return (insert(_Where, _Ilist.begin(), _Ilist.end()));
	}

	~vector() _NOEXCEPT
	{	// destroy the object
		_Tidy();
	}

	_Myt& operator=(const _Myt& _Right)
	{	// assign _Right
		if (this != &_Right)
		{	// different, assign it
			if (this->_Getal() != _Right._Getal()
				&& _Alty::propagate_on_container_copy_assignment::value)
			{	// change allocator before copying
				_Tidy();
				this->_Change_alloc(_Right._Getal());
			}

			this->_Orphan_all();

			if (_Right.empty())
				clear();	// new sequence empty, erase existing sequence
			else if (_Right.size() <= size())
			{	// enough elements, copy new and destroy old
				pointer _Ptr = _Copy_impl(_Right._Myfirst,
					_Right._Mylast, this->_Myfirst);	// copy new
				_Destroy(_Ptr, this->_Mylast);	// destroy old
				this->_Mylast = this->_Myfirst + _Right.size();
			}
			else if (_Right.size() <= capacity())
			{	// enough room, copy and construct new
				pointer _Ptr = _Right._Myfirst + size();
				_Copy_impl(_Right._Myfirst,
					_Ptr, this->_Myfirst);
				this->_Mylast = _Ucopy(_Ptr, _Right._Mylast, this->_Mylast);
			}
			else
			{	// not enough room, allocate new array and construct new
				if (this->_Myfirst != pointer())
				{	// discard old array
					_Destroy(this->_Myfirst, this->_Mylast);
					this->_Getal().deallocate(this->_Myfirst,
						this->_Myend - this->_Myfirst);
				}
				if (_Buy(_Right.size()))
					_TRY_BEGIN
					this->_Mylast = _Ucopy(_Right._Myfirst, _Right._Mylast,
					this->_Myfirst);
				_CATCH_ALL
					_Tidy();
				_RERAISE;
				_CATCH_END
			}
		}
		return (*this);
	}

	void reserve(size_type _Count)
	{	// determine new minimum length of allocated storage
		if (capacity() < _Count)
		{	// something to do, check and reallocate
			if (max_size() < _Count)
				_Xlen();
			_Reallocate(_Count);
		}
	}

	size_type capacity() const _NOEXCEPT
	{	// return current length of allocated storage
		return (this->_Myend - this->_Myfirst);
	}

	size_type _Unused_capacity() const _NOEXCEPT
	{	// micro-optimization for capacity() - size()
		return (this->_Myend - this->_Mylast);
	}

	size_type _Has_unused_capacity() const _NOEXCEPT
	{	// micro-optimization for capacity() != size()
		return (this->_Myend != this->_Mylast);
	}

	iterator begin() _NOEXCEPT
	{	// return iterator for beginning of mutable sequence
		return (iterator(this->_Myfirst, this));
	}

	const_iterator begin() const _NOEXCEPT
	{	// return iterator for beginning of nonmutable sequence
		return (const_iterator(this->_Myfirst, this));
	}

	iterator end() _NOEXCEPT
	{	// return iterator for end of mutable sequence
		return (iterator(this->_Mylast, this));
	}

	const_iterator end() const _NOEXCEPT
	{	// return iterator for end of nonmutable sequence
		return (const_iterator(this->_Mylast, this));
	}

	iterator _Make_iter(const_iterator _Where) const
	{	// make iterator from const_iterator
		return (iterator(_Where._Ptr, this));
	}

	reverse_iterator rbegin() _NOEXCEPT
	{	// return iterator for beginning of reversed mutable sequence
		return (reverse_iterator(end()));
	}

	const_reverse_iterator rbegin() const _NOEXCEPT
	{	// return iterator for beginning of reversed nonmutable sequence
		return (const_reverse_iterator(end()));
	}

	reverse_iterator rend() _NOEXCEPT
	{	// return iterator for end of reversed mutable sequence
		return (reverse_iterator(begin()));
	}

	const_reverse_iterator rend() const _NOEXCEPT
	{	// return iterator for end of reversed nonmutable sequence
		return (const_reverse_iterator(begin()));
	}

	const_iterator cbegin() const _NOEXCEPT
	{	// return iterator for beginning of nonmutable sequence
		return (((const _Myt *)this)->begin());
	}

	const_iterator cend() const _NOEXCEPT
	{	// return iterator for end of nonmutable sequence
		return (((const _Myt *)this)->end());
	}

	const_reverse_iterator crbegin() const _NOEXCEPT
	{	// return iterator for beginning of reversed nonmutable sequence
		return (((const _Myt *)this)->rbegin());
	}

	const_reverse_iterator crend() const _NOEXCEPT
	{	// return iterator for end of reversed nonmutable sequence
		return (((const _Myt *)this)->rend());
	}

	void shrink_to_fit()
	{	// reduce capacity
		if (_Has_unused_capacity())
		{	// worth shrinking, do it
			if (empty())
				_Tidy();
			else
				_Reallocate(size());
		}
	}

	void resize(size_type _Newsize)
	{	// determine new length, padding as needed
		if (_Newsize < size())
			_Pop_back_n(size() - _Newsize);
		else if (size() < _Newsize)
		{	// pad as needed
			_Alty _Alval(this->_Getal());
			_Reserve(_Newsize - size());
			_TRY_BEGIN
				_Uninitialized_default_fill_n(this->_Mylast, _Newsize - size(),
				_Alval);
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
				this->_Mylast += _Newsize - size();
		}
	}

	void resize(size_type _Newsize, const value_type& _Val)
	{	// determine new length, padding with _Val elements as needed
		if (_Newsize < size())
			_Pop_back_n(size() - _Newsize);
		else if (size() < _Newsize)
		{	// pad as needed
			const value_type *_Ptr = _STD addressof(_Val);

			if (_Inside(_Ptr))
			{	// padding is inside vector, recompute _Ptr after reserve
				const difference_type _Idx = _Ptr
					- _STD addressof(*this->_Myfirst);
				_Reserve(_Newsize - size());
				_Ptr = _STD addressof(*this->_Myfirst) + _Idx;
			}
			else
				_Reserve(_Newsize - size());

			_TRY_BEGIN
				_Ufill(this->_Mylast, _Newsize - size(), _Ptr);
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
				this->_Mylast += _Newsize - size();
		}
	}

	size_type size() const _NOEXCEPT
	{	// return length of sequence
		return (this->_Mylast - this->_Myfirst);
	}

	size_type max_size() const _NOEXCEPT
	{	// return maximum possible length of sequence
		return (this->_Getal().max_size());
	}

	bool empty() const _NOEXCEPT
	{	// test if sequence is empty
		return (this->_Myfirst == this->_Mylast);
	}

	_Alloc get_allocator() const _NOEXCEPT
	{	// return allocator object for values
		return (this->_Getal());
	}

	const_reference at(size_type _Pos) const
	{	// subscript nonmutable sequence with checking
		if (size() <= _Pos)
			_Xran();
		return (*(this->_Myfirst + _Pos));
	}

	reference at(size_type _Pos)
	{	// subscript mutable sequence with checking
		if (size() <= _Pos)
			_Xran();
		return (*(this->_Myfirst + _Pos));
	}

	const_reference operator[](size_type _Pos) const
	{	// subscript nonmutable sequence
#if _ITERATOR_DEBUG_LEVEL == 2
		if (size() <= _Pos)
		{	// report error
			_DEBUG_ERROR("vector subscript out of range");
			_SCL_SECURE_OUT_OF_RANGE;
		}

#elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE_RANGE(_Pos < size());
#endif /* _ITERATOR_DEBUG_LEVEL */

		return (*(this->_Myfirst + _Pos));
	}

	reference operator[](size_type _Pos)
	{	// subscript mutable sequence
#if _ITERATOR_DEBUG_LEVEL == 2
		if (size() <= _Pos)
		{	// report error
			_DEBUG_ERROR("vector subscript out of range");
			_SCL_SECURE_OUT_OF_RANGE;
		}

#elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE_RANGE(_Pos < size());
#endif /* _ITERATOR_DEBUG_LEVEL */

		return (*(this->_Myfirst + _Pos));
	}

	pointer data() _NOEXCEPT
	{	// return address of first element
		return (this->_Myfirst);
	}

	const_pointer data() const _NOEXCEPT
	{	// return address of first element
		return (this->_Myfirst);
	}

	reference front()
	{	// return first element of mutable sequence
		return (*begin());
	}

	const_reference front() const
	{	// return first element of nonmutable sequence
		return (*begin());
	}

	reference back()
	{	// return last element of mutable sequence
		return (*(end() - 1));
	}

	const_reference back() const
	{	// return last element of nonmutable sequence
		return (*(end() - 1));
	}

	void push_back(const value_type& _Val)
	{	// insert element at end
		if (_Inside(_STD addressof(_Val)))
		{	// push back an element
			size_type _Idx = _STD addressof(_Val) - this->_Myfirst;
			if (this->_Mylast == this->_Myend)
				_Reserve(1);
			_Orphan_range(this->_Mylast, this->_Mylast);
			this->_Getal().construct(this->_Mylast,
				this->_Myfirst[_Idx]);
			++this->_Mylast;
		}
		else
		{	// push back a non-element
			if (this->_Mylast == this->_Myend)
				_Reserve(1);
			_Orphan_range(this->_Mylast, this->_Mylast);
			this->_Getal().construct(this->_Mylast,
				_Val);
			++this->_Mylast;
		}
	}

#if _ITERATOR_DEBUG_LEVEL == 2
	void pop_back()
	{	// erase element at end
		if (empty())
			_DEBUG_ERROR("vector empty before pop");
		else
		{	// erase last element
			_Orphan_range(this->_Mylast - 1, this->_Mylast);
			this->_Getal().destroy(this->_Mylast - 1);
			--this->_Mylast;
		}
	}

#else /* _ITERATOR_DEBUG_LEVEL == 2 */
	void pop_back()
	{	// erase element at end
		this->_Getal().destroy(this->_Mylast - 1);
		--this->_Mylast;
	}
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

	template<class _Iter>
	typename enable_if<_Is_iterator<_Iter>::value,
		void>::type
		assign(_Iter _First, _Iter _Last)
	{	// assign [_First, _Last)
			clear();
			_Assign(_First, _Last, _Iter_cat(_First));
		}

	template<class _Iter>
	void _Assign(_Iter _First, _Iter _Last, input_iterator_tag)
	{	// assign [_First, _Last), input iterators
		for (; _First != _Last; ++_First)
			emplace_back(*_First);
	}

	template<class _Iter>
	void _Assign(_Iter _First, _Iter _Last, forward_iterator_tag)
	{	// assign [_First, _Last), forward iterators
		if (_First == _Last)
			return;	// nothing to do

		size_type _Newsize = _STD distance(_First, _Last);

		if (capacity() < _Newsize)
		{	// need more room, try to get it
			size_type _Newcapacity = _Grow_to(_Newsize);
			_Tidy();
			_Buy(_Newcapacity);
		}

		this->_Mylast = _Ucopy(_First, _Last, this->_Myfirst);
	}

	void assign(size_type _Count, const value_type& _Val)
	{	// assign _Count * _Val
		clear();
		insert(begin(), _Count, _Val);
	}

	iterator insert(const_iterator _Where, const _Ty& _Val)
	{	// insert _Val at _Where
		return (_Insert_n(_Where, (size_type)1, _Val));
	}

	iterator insert(const_iterator _Where, size_type _Count,
		const _Ty& _Val)
	{	// insert _Count * _Val at _Where
		return (_Insert_n(_Where, _Count, _Val));
	}

	template<class _Iter>
	typename enable_if<_Is_iterator<_Iter>::value,
		iterator>::type
		insert(const_iterator _Where, _Iter _First, _Iter _Last)
	{	// insert [_First, _Last) at _Where
			size_type _Off = _VIPTR(_Where) - this->_Myfirst;
			_Insert(_Where, _First, _Last, _Iter_cat(_First));
			return (begin() + _Off);
		}

	template<class _Iter>
	void _Insert(const_iterator _Where, _Iter _First, _Iter _Last,
		input_iterator_tag)
	{	// insert [_First, _Last) at _Where, input iterators
		size_type _Off = _VIPTR(_Where) - this->_Myfirst;

#if _ITERATOR_DEBUG_LEVEL == 2
		if (size() < _Off)
			_DEBUG_ERROR("vector insert iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		if (_First != _Last)
		{	// worth doing, gather at end and rotate into place
			size_type _Oldsize = size();

			_TRY_BEGIN
			for (; _First != _Last; ++_First)
				push_back(*_First);	// append

			_CATCH_ALL
				erase(begin() + _Oldsize, end());
			_RERAISE;
			_CATCH_END

				_STD rotate(begin() + _Off, begin() + _Oldsize, end());
		}
	}

	template<class _Iter>
	void _Insert(const_iterator _Where, _Iter _First, _Iter _Last,
		forward_iterator_tag)
	{	// insert [_First, _Last) at _Where, forward iterators
#if _ITERATOR_DEBUG_LEVEL == 2
		if (_VICONT(_Where) != this
			|| _VIPTR(_Where) < this->_Myfirst
			|| this->_Mylast < _VIPTR(_Where))
			_DEBUG_ERROR("vector insert iterator outside range");
		_DEBUG_RANGE(_First, _Last);
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		size_type _Count = 0;
		_Distance(_First, _Last, _Count);

		if (_Count == 0)
			;
		else if (_Unused_capacity() < _Count)
		{	// not enough room, reallocate
			if (max_size() - size() < _Count)
				_Xlen();	// result too long

			size_type _Capacity = _Grow_to(size() + _Count);
			pointer _Newvec = this->_Getal().allocate(_Capacity);
			pointer _Ptr = _Newvec;

			_TRY_BEGIN
				_Ptr = _Umove(this->_Myfirst, _VIPTR(_Where),
				_Newvec);	// copy prefix
			_Ptr = _Ucopy(_First, _Last, _Ptr);	// add new stuff
			_Umove(_VIPTR(_Where), this->_Mylast,
				_Ptr);	// copy suffix
			_CATCH_ALL
				_Destroy(_Newvec, _Ptr);
			this->_Getal().deallocate(_Newvec, _Capacity);
			_RERAISE;
			_CATCH_END

				_Count += size();
			if (this->_Myfirst != pointer())
			{	// destroy and deallocate old array
				_Destroy(this->_Myfirst, this->_Mylast);
				this->_Getal().deallocate(this->_Myfirst,
					this->_Myend - this->_Myfirst);
			}

			this->_Orphan_all();
			this->_Myend = _Newvec + _Capacity;
			this->_Mylast = _Newvec + _Count;
			this->_Myfirst = _Newvec;
		}
		else
		{	// new stuff fits, append and rotate into place
			_Ucopy(_First, _Last, this->_Mylast);
			_STD rotate(_VIPTR(_Where), this->_Mylast,
				this->_Mylast + _Count);
			this->_Mylast += _Count;
			_Orphan_range(_VIPTR(_Where), this->_Mylast);
		}
	}

#if _ITERATOR_DEBUG_LEVEL == 2
	iterator erase(const_iterator _Where)
	{	// erase element at where
		if (_VICONT(_Where) != this
			|| _VIPTR(_Where) < this->_Myfirst
			|| this->_Mylast <= _VIPTR(_Where))
			_DEBUG_ERROR("vector erase iterator outside range");
		_Move(_VIPTR(_Where) + 1, this->_Mylast, _VIPTR(_Where));
		_Destroy(this->_Mylast - 1, this->_Mylast);
		_Orphan_range(_VIPTR(_Where), this->_Mylast);
		--this->_Mylast;
		return (_Make_iter(_Where));
	}

#else /* _ITERATOR_DEBUG_LEVEL == 2 */
	iterator erase(const_iterator _Where)
	{	// erase element at where
		_Move(_VIPTR(_Where) + 1, this->_Mylast,
			_VIPTR(_Where));
		_Destroy(this->_Mylast - 1, this->_Mylast);
		--this->_Mylast;
		return (_Make_iter(_Where));
	}
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

	iterator erase(const_iterator _First_arg,
		const_iterator _Last_arg)
	{	// erase [_First, _Last)
		if (_First_arg == begin() && _Last_arg == end())
			clear();
		else if (_First_arg != _Last_arg)
		{	// clear partial
			iterator _First = _Make_iter(_First_arg);
			iterator _Last = _Make_iter(_Last_arg);

			if (_First != _Last)
			{	// worth doing, copy down over hole
#if _ITERATOR_DEBUG_LEVEL == 2
				if (_Last < _First || _VICONT(_First) != this
					|| _VIPTR(_First) < this->_Myfirst
					|| this->_Mylast < _VIPTR(_Last))
					_DEBUG_ERROR("vector erase iterator outside range");
				pointer _Ptr = _Move(_VIPTR(_Last), this->_Mylast,
					_VIPTR(_First));
				_Orphan_range(_VIPTR(_First), this->_Mylast);

#else /* _ITERATOR_DEBUG_LEVEL == 2 */
				pointer _Ptr = _Move(_VIPTR(_Last), this->_Mylast,
					_VIPTR(_First));
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

				_Destroy(_Ptr, this->_Mylast);
				this->_Mylast = _Ptr;
			}
		}
		return (_Make_iter(_First_arg));
	}

	void _Pop_back_n(size_type _Count)
	{	// erase _Count elements at end
		pointer _Ptr = this->_Mylast - _Count;

#if _ITERATOR_DEBUG_LEVEL == 2
		_Orphan_range(_Ptr, this->_Mylast);
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Destroy(_Ptr, this->_Mylast);
		this->_Mylast = _Ptr;
	}

	void clear() _NOEXCEPT
	{	// erase all
		this->_Orphan_all();
		_Destroy(this->_Myfirst, this->_Mylast);
		this->_Mylast = this->_Myfirst;
	}

	void swap(_Myt& _Right)
	{	// exchange contents with _Right
		if (this == &_Right)
			;	// same object, do nothing
		else if (this->_Getal() == _Right._Getal())
		{	// same allocator, swap control information
			this->_Swap_all(_Right);
			_Swap_adl(this->_Myfirst, _Right._Myfirst);
			_Swap_adl(this->_Mylast, _Right._Mylast);
			_Swap_adl(this->_Myend, _Right._Myend);
		}

		else if (_Alty::propagate_on_container_swap::value)
		{	// swap allocators and control information
			this->_Swap_alloc(_Right);
			_Swap_adl(this->_Myfirst, _Right._Myfirst);
			_Swap_adl(this->_Mylast, _Right._Mylast);
			_Swap_adl(this->_Myend, _Right._Myend);
		}

		else
		{	// containers are incompatible
#if _ITERATOR_DEBUG_LEVEL == 2
			_DEBUG_ERROR("vector containers incompatible for swap");

#else /* ITERATOR_DEBUG_LEVEL == 2 */
			_XSTD terminate();
#endif /* ITERATOR_DEBUG_LEVEL == 2 */
		}
	}

protected:
	bool _Buy(size_type _Capacity)
	{	// allocate array with _Capacity elements
		this->_Myfirst = pointer();
		this->_Mylast = pointer();
		this->_Myend = pointer();

		if (_Capacity == 0)
			return (false);
		else if (max_size() < _Capacity)
			_Xlen();	// result too long
		else
		{	// nonempty array, allocate storage
			this->_Myfirst = this->_Getal().allocate(_Capacity);
			this->_Mylast = this->_Myfirst;
			this->_Myend = this->_Myfirst + _Capacity;
		}
		return (true);
	}

	void _Destroy(pointer _First, pointer _Last)
	{	// destroy [_First, _Last) using allocator
		_Alty _Alval(this->_Getal());
		_Destroy_range(_First, _Last, _Alval);
	}

	size_type _Grow_to(size_type _Count) const
	{	// grow by 50% or at least to _Count
		size_type _Capacity = capacity();

		_Capacity = max_size() - _Capacity / 2 < _Capacity
			? 0 : _Capacity + _Capacity / 2;	// try to grow by 50%
		if (_Capacity < _Count)
			_Capacity = _Count;
		return (_Capacity);
	}

	bool _Inside(const value_type *_Ptr) const
	{	// test if _Ptr points inside vector
		return (_Ptr < this->_Mylast && this->_Myfirst <= _Ptr);
	}

	void _Reallocate(size_type _Count)
	{	// move to array of exactly _Count elements
		pointer _Ptr = this->_Getal().allocate(_Count);

		_TRY_BEGIN
			_Umove(this->_Myfirst, this->_Mylast, _Ptr);
		_CATCH_ALL
			this->_Getal().deallocate(_Ptr, _Count);
		_RERAISE;
		_CATCH_END

			size_type _Size = size();
		if (this->_Myfirst != pointer())
		{	// destroy and deallocate old array
			_Destroy(this->_Myfirst, this->_Mylast);
			this->_Getal().deallocate(this->_Myfirst,
				this->_Myend - this->_Myfirst);
		}

		this->_Orphan_all();
		this->_Myend = _Ptr + _Count;
		this->_Mylast = _Ptr + _Size;
		this->_Myfirst = _Ptr;
	}

	void _Reserve(size_type _Count)
	{	// ensure room for _Count new elements, grow exponentially
		if (_Unused_capacity() < _Count)
		{	// need more room, try to get it
			if (max_size() - size() < _Count)
				_Xlen();
			_Reallocate(_Grow_to(size() + _Count));
		}
	}

	void _Tidy()
	{	// free all storage
		if (this->_Myfirst != pointer())
		{	// something to free, destroy and deallocate it
			this->_Orphan_all();
			_Destroy(this->_Myfirst, this->_Mylast);
			this->_Getal().deallocate(this->_Myfirst,
				this->_Myend - this->_Myfirst);
			this->_Myfirst = pointer();
			this->_Mylast = pointer();
			this->_Myend = pointer();
		}
	}

	template<class _Iter>
	pointer _Ucopy(_Iter _First, _Iter _Last, pointer _Ptr)
	{	// copy initializing [_First, _Last), using allocator
		_Alty _Alval(this->_Getal());
		return (_Uninitialized_copy(_First, _Last,
			_Ptr, _Alval));
	}

	template<class _Iter>
	pointer _Umove(_Iter _First, _Iter _Last, pointer _Ptr)
	{	// move initializing [_First, _Last), using allocator
		_Alty _Alval(this->_Getal());
		return (_Uninitialized_move(_First, _Last,
			_Ptr, _Alval));
	}

	iterator _Insert_n(const_iterator _Where,
		size_type _Count, const value_type& _Val)
	{	// insert _Count * _Val at _Where
#if _ITERATOR_DEBUG_LEVEL == 2
		if (_VICONT(_Where) != this
			|| _VIPTR(_Where) < this->_Myfirst
			|| this->_Mylast < _VIPTR(_Where))
			_DEBUG_ERROR("vector insert iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		size_type _Off = _VIPTR(_Where) - this->_Myfirst;
		if (_Count == 0)
			;
		else if (_Unused_capacity() < _Count)
		{	// not enough room, reallocate
			if (max_size() - size() < _Count)
				_Xlen();	// result too long

			size_type _Capacity = _Grow_to(size() + _Count);
			pointer _Newvec = this->_Getal().allocate(_Capacity);
			size_type _Whereoff = _VIPTR(_Where) - this->_Myfirst;
			int _Ncopied = 0;

			_TRY_BEGIN
				_Ufill(_Newvec + _Whereoff, _Count,
				_STD addressof(_Val));	// add new stuff
			++_Ncopied;
			_Umove(this->_Myfirst, _VIPTR(_Where),
				_Newvec);	// copy prefix
			++_Ncopied;
			_Umove(_VIPTR(_Where), this->_Mylast,
				_Newvec + (_Whereoff + _Count));	// copy suffix
			_CATCH_ALL
			if (1 < _Ncopied)
				_Destroy(_Newvec, _Newvec + _Whereoff);
			if (0 < _Ncopied)
				_Destroy(_Newvec + _Whereoff, _Newvec + _Whereoff + _Count);
			this->_Getal().deallocate(_Newvec, _Capacity);
			_RERAISE;
			_CATCH_END

				_Count += size();
			if (this->_Myfirst != pointer())
			{	// destroy and deallocate old array
				_Destroy(this->_Myfirst, this->_Mylast);
				this->_Getal().deallocate(this->_Myfirst,
					this->_Myend - this->_Myfirst);
			}

			this->_Orphan_all();
			this->_Myend = _Newvec + _Capacity;
			this->_Mylast = _Newvec + _Count;
			this->_Myfirst = _Newvec;
		}
		else if ((size_type)(this->_Mylast - _VIPTR(_Where))
			< _Count)
		{	// new stuff spills off end
			value_type _Tmp = _Val;	// in case _Val is in sequence

			_Umove(_VIPTR(_Where), this->_Mylast,
				_VIPTR(_Where) + _Count);	// copy suffix

			_TRY_BEGIN
				_Ufill(this->_Mylast,
				_Count - (this->_Mylast - _VIPTR(_Where)),
				_STD addressof(_Tmp));	// insert new stuff off end
			_CATCH_ALL
				_Destroy(_VIPTR(_Where) + _Count,
				this->_Mylast + _Count);
			_RERAISE;
			_CATCH_END

				this->_Mylast += _Count;
			_Orphan_range(_VIPTR(_Where), this->_Mylast);
			_STD fill(_VIPTR(_Where), this->_Mylast - _Count,
				_Tmp);	// insert up to old end
		}
		else
		{	// new stuff can all be assigned
			value_type _Tmp = _Val;	// in case _Val is in sequence

			pointer _Oldend = this->_Mylast;
			this->_Mylast = _Umove(_Oldend - _Count, _Oldend,
				this->_Mylast);	// copy suffix

			_Orphan_range(_VIPTR(_Where), this->_Mylast);
			_Copy_backward(_VIPTR(_Where), _Oldend - _Count,
				_Oldend);	// copy hole
			_STD fill(_VIPTR(_Where),
				_VIPTR(_Where) + _Count, _Tmp);	// insert into hole
		}
		return (begin() + _Off);
	}

	pointer _Ufill(pointer _Ptr, size_type _Count, const value_type *_Pval)
	{	// copy initializing _Count * _Val, using allocator
		_Alty _Alval(this->_Getal());
		_Uninitialized_fill_n(_Ptr, _Count, _Pval, _Alval);
		return (_Ptr + _Count);
	}

	__declspec(noreturn) void _Xlen() const
	{	// report a length_error
		_Xlength_error("vector<T> too long");
	}

	__declspec(noreturn) void _Xran() const
	{	// report an out_of_range error
		_Xout_of_range("invalid vector<T> subscript");
	}

#if _VECTOR_ORPHAN_RANGE
	void _Orphan_range(pointer _First, pointer _Last) const
	{	// orphan iterators within specified (inclusive) range
		_Lockit _Lock(_LOCK_DEBUG);
		const_iterator **_Pnext = (const_iterator **)this->_Getpfirst();
		if (_Pnext != 0)
		while (*_Pnext != 0)
		if ((*_Pnext)->_Ptr < _First || _Last < (*_Pnext)->_Ptr)
			_Pnext = (const_iterator **)(*_Pnext)->_Getpnext();
		else
		{	// orphan the iterator
			(*_Pnext)->_Clrcont();
			*_Pnext = *(const_iterator **)(*_Pnext)->_Getpnext();
		}
	}

#else /* _VECTOR_ORPHAN_RANGE */
	void _Orphan_range(pointer, pointer) const
	{	// orphan iterators within specified (inclusive) range
	}
#endif /* _VECTOR_ORPHAN_RANGE */
};
