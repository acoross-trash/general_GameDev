#pragma once

template<class _Alloc>
struct _Wrap_alloc
	: public _Alloc
{	// defines traits for allocators
	typedef _Wrap_alloc<_Alloc> other;

	typedef _Alloc _Mybase;
	typedef allocator_traits<_Alloc> _Mytraits;

	typedef typename _Mytraits::value_type value_type;

	typedef typename _Mytraits::pointer pointer;
	typedef typename _Mytraits::const_pointer const_pointer;
	typedef typename _Mytraits::void_pointer void_pointer;
	typedef typename _Mytraits::const_void_pointer const_void_pointer;

	typedef typename _If<is_void<value_type>::value,
		int, value_type>::type& reference;
	typedef typename _If<is_void<const value_type>::value,
		const int, const value_type>::type& const_reference;

	typedef typename _Mytraits::size_type size_type;
	typedef typename _Mytraits::difference_type difference_type;

	typedef typename _Mytraits::propagate_on_container_copy_assignment
		propagate_on_container_copy_assignment;
	typedef typename _Mytraits::propagate_on_container_move_assignment
		propagate_on_container_move_assignment;
	typedef typename _Mytraits::propagate_on_container_swap
		propagate_on_container_swap;

	_Wrap_alloc select_on_container_copy_construction() const
	{	// get allocator to use
		return (_Mytraits::select_on_container_copy_construction(*this));
	}

	template<class _Other>
	struct rebind
	{	// convert this type to allocator<_Other>
		typedef typename _Mytraits::template rebind_alloc<_Other> _Other_alloc;
		typedef _Wrap_alloc<_Other_alloc> other;
	};

	pointer address(reference _Val) const
	{	// return address of mutable _Val
		return (_STD addressof(_Val));
	}

	const_pointer address(const_reference _Val) const
	{	// return address of nonmutable _Val
		return (_STD addressof(_Val));
	}

	_Wrap_alloc() _THROW0()
		: _Mybase()
	{	// construct default allocator (do nothing)
	}

	_Wrap_alloc(const _Mybase& _Right) _THROW0()
		: _Mybase(_Right)
	{	// construct by copying base
	}

	_Wrap_alloc(const _Wrap_alloc& _Right) _THROW0()
		: _Mybase(_Right)
	{	// construct by copying
	}

	template<class _Other>
	_Wrap_alloc(const _Other& _Right) _THROW0()
		: _Mybase(_Right)
	{	// construct from a related allocator
	}

	template<class _Other>
	_Wrap_alloc(_Other& _Right) _THROW0()
		: _Mybase(_Right)
	{	// construct from a related allocator
	}

	_Wrap_alloc& operator=(const _Mybase& _Right)
	{	// construct by copying base
		_Mybase::operator=(_Right);
		return (*this);
	}

	_Wrap_alloc& operator=(const _Wrap_alloc& _Right)
	{	// construct by copying
		_Mybase::operator=(_Right);
		return (*this);
	}

	template<class _Other>
	_Wrap_alloc& operator=(const _Other& _Right)
	{	// assign from a related allocator
		_Mybase::operator=(_Right);
		return (*this);
	}

	pointer allocate(size_type _Count)
	{	// allocate array of _Count elements
		return (_Mybase::allocate(_Count));
	}

	pointer allocate(size_type _Count,
		const_void_pointer _Hint)
	{	// allocate array of _Count elements, with hint
		return (_Mytraits::allocate(*this, _Count, _Hint));
	}

	void deallocate(pointer _Ptr, size_type _Count)
	{	// deallocate object at _Ptr, ignore size
		_Mybase::deallocate(_Ptr, _Count);
	}

	void construct(value_type *_Ptr)
	{	// default construct object at _Ptr
		_Mytraits::construct(*this, _Ptr);
	}

	template<class _Ty,
	class... _Types>
		void construct(_Ty *_Ptr,
		_Types&&... _Args)
	{	// construct _Ty(_Types...) at _Ptr
			_Mytraits::construct(*this, _Ptr,
				_STD forward<_Types>(_Args)...);
		}


	template<class _Ty>
	void destroy(_Ty *_Ptr)
	{	// destroy object at _Ptr
		_Mytraits::destroy(*this, _Ptr);
	}

	size_type max_size() const _THROW0()
	{	// get maximum size
		return (_Mytraits::max_size(*this));
	}
};