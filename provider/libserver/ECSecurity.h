/*
 * Copyright 2005 - 2016 Zarafa and its licensors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// ECSecurity.h: interface for the ECSecurity class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ECSECURITY
#define ECSECURITY

#include "ECUserManagement.h"
#include "plugin.h"

#include <kopano/ECLogger.h>
#include <kopano/ECConfig.h>
#include <kopano/ECDefs.h>

namespace KC {

class ECSession;

#define EC_NO_IMPERSONATOR		((unsigned int)-1)

class ECSecurity _kc_final {
public:
	ECSecurity(ECSession *lpSession, ECConfig *lpConfig, ECLogger *lpAudit);
	virtual ~ECSecurity();

	/* must be called once the object is created */
	virtual ECRESULT SetUserContext(unsigned int ulUserId, unsigned int ulImpersonatorID);
	virtual ECRESULT CheckDeletedParent(unsigned int id) const;
	virtual ECRESULT CheckPermission(unsigned int ulObjId, unsigned int ulCheckRights);

	virtual ECRESULT GetRights(unsigned int objid, int type, struct rightsArray *out) const;
	virtual ECRESULT SetRights(unsigned int objid, struct rightsArray *lpsRightsArray);
	virtual ECRESULT GetUserCompany(unsigned int *) const;

	// Functions to determine which companies are visible, and which userobjects are
	// visible to the currently logged in user.
	virtual ECRESULT GetViewableCompanyIds(unsigned int ulFlags, std::list<localobjectdetails_t> **lppObjects);
	virtual ECRESULT IsUserObjectVisible(unsigned int ulUserObjectId);

	// Get the owner of an object
	virtual ECRESULT GetOwner(unsigned int obj_id, unsigned int *owner) const;

	// get the store owner, you can give every object id
	virtual ECRESULT GetStoreOwner(unsigned int obj_id, unsigned int *owner) const;
	virtual ECRESULT GetStoreOwnerAndType(unsigned int obj_id, unsigned int *owner, unsigned int *store_type) const;
	virtual ECRESULT GetObjectPermission(unsigned int id, unsigned int *rights) __attribute__((nonnull));
	virtual unsigned int GetUserId(unsigned int ulObjId = 0);
	virtual ECRESULT IsOwner(unsigned int objid) const;
	virtual ECRESULT IsStoreOwner(unsigned int store_id) const;
	virtual int GetAdminLevel(void) const;

	// Functions to determine if the user is the Administrator
	// over the company to which the user/object/store belongs.
	virtual ECRESULT IsAdminOverUserObject(unsigned int ulUserObjectId);
	virtual ECRESULT IsAdminOverOwnerOfObject(unsigned int ulObjectId);

	// Quota functions
	virtual ECRESULT CheckQuota(unsigned int store_id, long long store_size, eQuotaStatus *) const;
	virtual ECRESULT CheckUserQuota(unsigned int user_id, long long store_size, eQuotaStatus *) const;
	virtual ECRESULT GetStoreSize(unsigned int obj_id, long long *store_size) const;
	virtual ECRESULT GetUserSize(unsigned int user_id, long long *user_size) const;
	virtual ECRESULT GetUserQuota(unsigned int user_id, bool usr_dfl, quotadetails_t *) const;

	// information for ECSessionStatsTable
	virtual ECRESULT GetUsername(std::string *) const;
	virtual ECRESULT GetImpersonator(std::string *) const;
	virtual size_t GetObjectSize(void) const;

private:
	ECRESULT GetGroupsForUser(unsigned int ulUserId, std::list<localobjectdetails_t> **lppGroups);
	ECRESULT GetViewableCompanies(unsigned int ulFlags, std::list<localobjectdetails_t> **lppObjects) const;
	ECRESULT GetAdminCompanies(unsigned int ulFlags, std::list<localobjectdetails_t> **lppObjects);
	ECRESULT HaveObjectPermission(unsigned int ulObjId, unsigned int ulACLMask);

protected:
	ECSession			*m_lpSession;
	ECLogger			*m_lpAudit;
	ECConfig			*m_lpConfig;

	unsigned int m_ulUserID = 0; // current user id
	unsigned int m_ulImpersonatorID = 0; // id of user that is impersonating the current user
	unsigned int m_ulCompanyID = 0; // Company to which the user belongs to
	objectdetails_t		m_details;
    objectdetails_t 	m_impersonatorDetails;
	bool				m_bRestrictedAdmin; // True if restricted admin permissions enabled
	bool 				m_bOwnerAutoFullAccess;
	std::list<localobjectdetails_t> *m_lpGroups = nullptr; // current user groups
	std::list<localobjectdetails_t> *m_lpViewCompanies = nullptr; // current visible companies
	std::list<localobjectdetails_t> *m_lpAdminCompanies = nullptr; // Companies where the user has admin rights on
};

} /* namespace */

#endif // #ifndef ECSECURITY
