#pragma once

template<class _Ty>
class allocator
	: public _Allocator_base<_Ty>
{	// generic allocator for objects of class _Ty
public:
	typedef allocator<_Ty> other;

	typedef _Allocator_base<_Ty> _Mybase;
	typedef typename _Mybase::value_type value_type;

	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef void *void_pointer;
	typedef const void *const_void_pointer;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef false_type propagate_on_container_copy_assignment;
	typedef false_type propagate_on_container_move_assignment;
	typedef false_type propagate_on_container_swap;

	allocator<_Ty> select_on_container_copy_construction() const
	{	// return this allocator
		return (*this);
	}

	template<class _Other>
	struct rebind
	{	// convert this type to allocator<_Other>
		typedef allocator<_Other> other;
	};

	pointer address(reference _Val) const _NOEXCEPT
	{	// return address of mutable _Val
		return (_STD addressof(_Val));
	}

	const_pointer address(const_reference _Val) const _NOEXCEPT
	{	// return address of nonmutable _Val
		return (_STD addressof(_Val));
	}

	allocator() _THROW0()
	{	// construct default allocator (do nothing)
	}

	allocator(const allocator<_Ty>&) _THROW0()
	{	// construct by copying (do nothing)
	}

	template<class _Other>
	allocator(const allocator<_Other>&) _THROW0()
	{	// construct from a related allocator (do nothing)
	}

	template<class _Other>
	allocator<_Ty>& operator=(const allocator<_Other>&)
	{	// assign from a related allocator (do nothing)
		return (*this);
	}

	void deallocate(pointer _Ptr, size_type)
	{	// deallocate object at _Ptr, ignore size
		::operator delete(_Ptr);
	}

	pointer allocate(size_type _Count)
	{	// allocate array of _Count elements
		return (_Allocate(_Count, (pointer)0));
	}

	pointer allocate(size_type _Count, const void *)
	{	// allocate array of _Count elements, ignore hint
		return (allocate(_Count));
	}

	void construct(_Ty *_Ptr)
	{	// default construct object at _Ptr
		::new ((void *)_Ptr) _Ty();
	}

	void construct(_Ty *_Ptr, const _Ty& _Val)
	{	// construct object at _Ptr with value _Val
		::new ((void *)_Ptr) _Ty(_Val);
	}

	template<class _Objty,
	class... _Types>
		void construct(_Objty *_Ptr, _Types&&... _Args)
	{	// construct _Objty(_Types...) at _Ptr
			::new ((void *)_Ptr) _Objty(_STD forward<_Types>(_Args)...);
		}


	template<class _Uty>
	void destroy(_Uty *_Ptr)
	{	// destroy object at _Ptr
		_Ptr->~_Uty();
	}

	size_t max_size() const _THROW0()
	{	// estimate maximum array size
		return ((size_t)(-1) / sizeof (_Ty));
	}
};