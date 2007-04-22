/*
 * im.h - XMPP "IM" library API
 * Copyright (C) 2003  Justin Karneges
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef XMPP_IM_H
#define XMPP_IM_H

#include <qdatetime.h>
//Added by qt3to4:
#include <QList>

#include "xmpp.h"
#include "xmpp_jid.h"
#include "xmpp_muc.h"
#include "xmpp_status.h"
#include "xmpp_features.h"
#include "xmpp_task.h"
#include "xmpp_rosterx.h"
#include "xmpp_xdata.h"
#include "xmpp_discoitem.h"
#include "xmpp_agentitem.h"
#include "xmpp_client.h"

namespace XMPP
{
	class Url
	{
	public:
		Url(const QString &url="", const QString &desc="");
		Url(const Url &);
		Url & operator=(const Url &);
		~Url();

		QString url() const;
		QString desc() const;

		void setUrl(const QString &);
		void setDesc(const QString &);

	private:
		class Private;
		Private *d;
	};

	class Address
	{
	public:
		typedef enum { Unknown, To, Cc, Bcc, ReplyTo, ReplyRoom, NoReply, OriginalFrom, OriginalTo } Type;

		Address(Type type = Unknown, const Jid& jid = Jid());
		Address(const QDomElement&);

		const Jid& jid() const;
		const QString& uri() const;
		const QString& node() const;
		const QString& desc() const;
		bool delivered() const;
		Type type() const;
		
		QDomElement toXml(Stanza&) const;
		void fromXml(const QDomElement& t);

		void setJid(const Jid &);
		void setUri(const QString &);
		void setNode(const QString &);
		void setDesc(const QString &);
		void setDelivered(bool);
		void setType(Type);

	private:
		Jid v_jid;
		QString v_uri, v_node, v_desc;
		bool v_delivered;
		Type v_type;
	};
	
	typedef QList<Url> UrlList;
	typedef QList<Address> AddressList;
	typedef QMap<QString, QString> StringMap;
	typedef enum { OfflineEvent, DeliveredEvent, DisplayedEvent,
			ComposingEvent, CancelEvent } MsgEvent;
	typedef enum { StateNone, StateActive, StateComposing, StatePaused, 
			StateInactive, StateGone } ChatState;
	
	class PubSubItem 
	{
	public:
		PubSubItem();
		PubSubItem(const QString& id, const QDomElement& payload);
		const QString& id() const;
		const QDomElement& payload() const;

	private:
		QString id_;
		QDomElement payload_;
	};

	class HTMLElement
	{
	public:
		HTMLElement();
		HTMLElement(const QDomElement &body);

		void setBody(const QDomElement &body);
		const QDomElement& body() const;
		QString toString(const QString &rootTagName = "body") const;
		QString text() const;

	private:
		QDomElement body_;
	};

	class HttpAuthRequest
	{
	public:
		HttpAuthRequest(const QString &m, const QString &u, const QString &i);
		HttpAuthRequest(const QString &m = "", const QString &u = "");
		HttpAuthRequest(const QDomElement &);

		bool isEmpty() const;

		void setMethod(const QString&);
		void setUrl(const QString&);
		void setId(const QString&);
		QString method() const;
		QString url() const;
		QString id() const;
		bool hasId() const;

		QDomElement toXml(QDomDocument &) const;
		bool fromXml(const QDomElement &);

		static Stanza::Error denyError;
	private:
		QString method_, url_, id_;
		bool hasId_;
	};

	class Message
	{
	public:
		Message(const Jid &to="");
		Message(const Message &from);
		Message & operator=(const Message &from);
		~Message();

		Jid to() const;
		Jid from() const;
		QString id() const;
		QString type() const;
		QString lang() const;
		QString subject(const QString &lang="") const;
		QString body(const QString &lang="") const;
		QString thread() const;
		Stanza::Error error() const;

		void setTo(const Jid &j);
		void setFrom(const Jid &j);
		void setId(const QString &s);
		void setType(const QString &s);
		void setLang(const QString &s);
		void setSubject(const QString &s, const QString &lang="");
		void setBody(const QString &s, const QString &lang="");
		void setThread(const QString &s, bool send = false);
		void setError(const Stanza::Error &err);

		// JEP-0060
		const QString& pubsubNode() const;
		const QList<PubSubItem>& pubsubItems() const;

		// JEP-0091
		QDateTime timeStamp() const;
		void setTimeStamp(const QDateTime &ts, bool send = false);

		// JEP-0071
		HTMLElement html(const QString &lang="") const;
		void setHTML(const HTMLElement &s, const QString &lang="");
		bool containsHTML() const;

		// JEP-0066
		UrlList urlList() const;
		void urlAdd(const Url &u);
		void urlsClear();
		void setUrlList(const UrlList &list);

		// JEP-0022
		QString eventId() const;
		void setEventId(const QString& id);
		bool containsEvents() const;
		bool containsEvent(MsgEvent e) const;
		void addEvent(MsgEvent e);

		// JEP-0085
		ChatState chatState() const;
		void setChatState(ChatState);
 
		// JEP-0027
		QString xencrypted() const;
		void setXEncrypted(const QString &s);
		
		// JEP-0033
		AddressList addresses() const;
		AddressList findAddresses(Address::Type t) const;
		void addAddress(const Address &a);
		void clearAddresses();
		void setAddresses(const AddressList &list);

		// JEP-144
		const RosterExchangeItems& rosterExchangeItems() const;
		void setRosterExchangeItems(const RosterExchangeItems&);

		// JEP-172
		void setNick(const QString&);
		const QString& nick() const;

		// JEP-0070
		void setHttpAuthRequest(const HttpAuthRequest&);
		HttpAuthRequest httpAuthRequest() const;

		// JEP-0004
		void setForm(const XData&);
		const XData& getForm() const;

		// JEP-xxxx Whiteboarding
		void setWhiteboard(const QDomElement&);
		const QDomElement& whiteboard() const;

		// MUC
		void setMUCStatus(int);
		bool hasMUCStatus() const;
		int mucStatus() const;
		void addMUCInvite(const MUCInvite&);
		const QList<MUCInvite>& mucInvites() const;
		void setMUCDecline(const MUCDecline&);
		const MUCDecline& mucDecline() const;
		const QString& mucPassword() const;
		void setMUCPassword(const QString&);

		// Obsolete invitation
		QString invite() const;
		void setInvite(const QString &s);

		// for compatibility.  delete me later
		bool spooled() const;
		void setSpooled(bool);
		bool wasEncrypted() const;
		void setWasEncrypted(bool);

		Stanza toStanza(Stream *stream) const;
		bool fromStanza(const Stanza &s, int tzoffset);

	private:
		class Private;
		Private *d;
	};

	class Subscription
	{
	public:
		enum SubType { None, To, From, Both, Remove };

		Subscription(SubType type=None);

		int type() const;

		QString toString() const;
		bool fromString(const QString &);

	private:
		SubType value;
	};

	class Resource
	{
	public:
		Resource(const QString &name="", const Status &s=Status());
		~Resource();

		const QString & name() const;
		int priority() const;
		const Status & status() const;

		void setName(const QString &);
		void setStatus(const Status &);

	private:
		QString v_name;
		Status v_status;
	};

	class ResourceList : public QList<Resource>
	{
	public:
		ResourceList();
		~ResourceList();

		ResourceList::Iterator find(const QString &);
		ResourceList::Iterator priority();

		ResourceList::ConstIterator find(const QString &) const;
		ResourceList::ConstIterator priority() const;
	};

	class RosterItem
	{
	public:
		RosterItem(const Jid &jid="");
		virtual ~RosterItem();

		const Jid & jid() const;
		const QString & name() const;
		const QStringList & groups() const;
		const Subscription & subscription() const;
		const QString & ask() const;
		bool isPush() const;
		bool inGroup(const QString &) const;

		virtual void setJid(const Jid &);
		void setName(const QString &);
		void setGroups(const QStringList &);
		void setSubscription(const Subscription &);
		void setAsk(const QString &);
		void setIsPush(bool);
		bool addGroup(const QString &);
		bool removeGroup(const QString &);

		QDomElement toXml(QDomDocument *) const;
		bool fromXml(const QDomElement &);

	private:
		Jid v_jid;
		QString v_name;
		QStringList v_groups;
		Subscription v_subscription;
		QString v_ask;
		bool v_push;
	};

	class Roster : public QList<RosterItem>
	{
	public:
		Roster();
		~Roster();

		Roster::Iterator find(const Jid &);
		Roster::ConstIterator find(const Jid &) const;

	private:
		class RosterPrivate *d;
	};

	typedef QList<AgentItem> AgentList;
	typedef QList<DiscoItem> DiscoList;

	class FormField
	{
	public:
		enum { username, nick, password, name, first, last, email, address, city, state, zip, phone, url, date, misc };
		FormField(const QString &type="", const QString &value="");
		~FormField();

		int type() const;
		QString fieldName() const;
		QString realName() const;
		bool isSecret() const;
		const QString & value() const;
		void setType(int);
		bool setType(const QString &);
		void setValue(const QString &);

	private:
		int tagNameToType(const QString &) const;
		QString typeToTagName(int) const;

		int v_type;
		QString v_value;

		class Private;
		Private *d;
	};

	class Form : public QList<FormField>
	{
	public:
		Form(const Jid &j="");
		~Form();

		Jid jid() const;
		QString instructions() const;
		QString key() const;
		void setJid(const Jid &);
		void setInstructions(const QString &);
		void setKey(const QString &);

	private:
		Jid v_jid;
		QString v_instructions, v_key;

		class Private;
		Private *d;
	};

	class SearchResult
	{
	public:
		SearchResult(const Jid &jid="");
		~SearchResult();

		const Jid & jid() const;
		const QString & nick() const;
		const QString & first() const;
		const QString & last() const;
		const QString & email() const;

		void setJid(const Jid &);
		void setNick(const QString &);
		void setFirst(const QString &);
		void setLast(const QString &);
		void setEmail(const QString &);

	private:
		Jid v_jid;
		QString v_nick, v_first, v_last, v_email;
	};

	class LiveRosterItem;
	class LiveRoster;
	class S5BManager;
	class IBBManager;
	class JidLinkManager;
	class FileTransferManager;

	class LiveRosterItem : public RosterItem
	{
	public:
		LiveRosterItem(const Jid &j="");
		LiveRosterItem(const RosterItem &);
		~LiveRosterItem();

		void setRosterItem(const RosterItem &);

		ResourceList & resourceList();
		ResourceList::Iterator priority();

		const ResourceList & resourceList() const;
		ResourceList::ConstIterator priority() const;

		bool isAvailable() const;
		const Status & lastUnavailableStatus() const;
		bool flagForDelete() const;

		void setLastUnavailableStatus(const Status &);
		void setFlagForDelete(bool);

	private:
		ResourceList v_resourceList;
		Status v_lastUnavailableStatus;
		bool v_flagForDelete;
	};

	class LiveRoster : public QList<LiveRosterItem>
	{
	public:
		LiveRoster();
		~LiveRoster();

		void flagAllForDelete();
		LiveRoster::Iterator find(const Jid &, bool compareRes=true);
		LiveRoster::ConstIterator find(const Jid &, bool compareRes=true) const;
	};
}

#endif
