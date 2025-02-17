//
// Copyright (c) 2021-2022, Manticore Software LTD (http://manticoresearch.com)
// All rights reserved
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License. You should have
// received a copy of the GPL license along with this program; if you
// did not, you can find it at http://www.gnu.org
//

#pragma once

#include <boost/intrusive/slist.hpp>
#include <boost/fiber/detail/spinlock.hpp>

namespace Threads
{
namespace Coro
{

class Worker_c;

namespace detail
{
using waker_queue_hook_t = boost::intrusive::slist_member_hook<> ;
}

class Waker_c
{
	Worker_c* m_pCtx {};
	size_t m_iEpoch {};

public:
	friend class Context_c;

	Waker_c() = default;

	Waker_c ( Worker_c* pCtx, const size_t iEpoch )
		: m_pCtx { pCtx }
		, m_iEpoch { iEpoch }
	{}

	bool Wake() const noexcept;
};

class WakerInQueue_c: public Waker_c
{
public:
	explicit WakerInQueue_c ( Waker_c&& w )
		: Waker_c { w }
	{}

	bool is_linked() const noexcept
	{
		return m_tWakerQueueHook.is_linked();
	}

	friend bool operator== ( Waker_c const& lhs, Waker_c const& rhs ) noexcept
	{
		return &lhs == &rhs;
	}

public:
	detail::waker_queue_hook_t m_tWakerQueueHook {};
};


namespace detail
{
using WakerSlist_t = boost::intrusive::slist<
		WakerInQueue_c,
		boost::intrusive::member_hook<
				WakerInQueue_c,
				detail::waker_queue_hook_t,
				&WakerInQueue_c::m_tWakerQueueHook>,
		boost::intrusive::constant_time_size<false>,
		boost::intrusive::cache_last<true>>;
}

class WaitQueue_c
{
private:
	detail::WakerSlist_t m_Slist {};

public:
	void SuspendAndWait ( boost::fibers::detail::spinlock_lock&, Worker_c* );
	void NotifyOne();
	void NotifyAll();

	bool Empty() const;
};

} // namespace Coro
} // namespace Threads
