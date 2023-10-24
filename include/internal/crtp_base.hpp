/**
 * @file crtp.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief CRTP base template
 *
 * @author s3mat3
 */

#pragma once

#ifndef CRTP_BASE_Hpp
# define  CRTP_BASE_Hpp

namespace Mult {
    namespace Internal {
        template <typename P>
        class crtp_base
        {
        public:
            using derived_type = typename P::derived_type;
            using derived_ptr  = derived_type*;
        protected:
            derived_type& derived() noexcept
            {
                return static_cast<derived_type&>(*this);
            }
            const derived_type& derived() const noexcept
            {
                return static_cast<const derived_type&>(*this);
            }
            derived_ptr ptr() noexcept {return static_cast<derived_type*>(this);}
        private:
        };
    } //<-- namespace Internal ends here.
} //<-- namespace Mult ends here.

#endif //<-- macro  CRTP_BASE_Hpp ends here.
