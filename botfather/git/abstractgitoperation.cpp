#include "abstractgitoperation.hpp"
#include <QCoreApplication>
#include <QDebug>

AbstractGitOperation::AbstractGitOperation()
{
	// Emit ::finished whenever ::failure or ::success are emitted
	connect(this, &AbstractGitOperation::failure, this, &AbstractGitOperation::finished);
	connect(this, &AbstractGitOperation::success, this, &AbstractGitOperation::finished);
}

AbstractGitOperation::~AbstractGitOperation()
{

}

bool AbstractGitOperation::isCanceled() const
{
	return m_is_canceled;
}

uint AbstractGitOperation::totalObjects() const
{
	return m_total_objects;
}

void AbstractGitOperation::setTotalObjects(uint objects)
{
	m_total_objects = objects;
}

uint AbstractGitOperation::receivedObjects() const
{
	return m_received_objects;
}

void AbstractGitOperation::setReceivedObjects(uint objects)
{
	m_received_objects = objects;
}

ulong AbstractGitOperation::receivedBytes() const
{
	return m_received_bytes;
}

void AbstractGitOperation::setReceivedBytes(ulong bytes)
{
	m_received_bytes = bytes;
}

ulong AbstractGitOperation::completedSteps() const
{
	return m_completed_steps;
}

void AbstractGitOperation::setCompletedSteps(ulong steps)
{
	m_completed_steps = steps;
}

ulong AbstractGitOperation::totalSteps() const
{
	return m_total_steps;
}

void AbstractGitOperation::setTotalSteps(ulong steps)
{
	m_total_steps = steps;
}

QString AbstractGitOperation::checkoutPath() const
{
	return m_checkout_path;
}

void AbstractGitOperation::setCheckoutPath(const QString &path)
{
	m_checkout_path = path;
}

void AbstractGitOperation::cancel()
{
	m_is_canceled = true;
}

int AbstractGitOperation::transferProgressCallback(const git_transfer_progress *stats, void *abstract_git_operation_p)
{
	AbstractGitOperation *operation = qobject_cast<AbstractGitOperation*>(static_cast<QObject*>(abstract_git_operation_p));
	QCoreApplication::processEvents(); // Without it the worker will miss cancel requests

	if (operation->isCanceled())
	{
		return CANCELED_CODE;
	}

	if (stats->total_objects != operation->totalObjects())
	{
		operation->setTotalObjects(stats->total_objects);
		emit operation->totalObjectsChanged(stats->total_objects);
	}

	if (stats->received_objects != operation->receivedObjects())
	{
		operation->setReceivedObjects(stats->received_objects);
		emit operation->receivedObjectsChanged(stats->received_objects);
	}

	if (stats->received_bytes != operation->receivedBytes())
	{
		operation->setReceivedBytes(stats->received_bytes);
		emit operation->transferProgressChanged(stats->received_objects, stats->total_objects, stats->received_bytes);
	}

	return 0;
}

void AbstractGitOperation::checkoutProgressCallback(const char *path, size_t completed_steps, size_t total_steps, void *abstract_git_operation_p)
{
	AbstractGitOperation *operation = qobject_cast<AbstractGitOperation*>(static_cast<QObject*>(abstract_git_operation_p));
	QCoreApplication::processEvents(); // Without it the operation will miss cancel requests

	if (completed_steps != operation->completedSteps())
	{
		operation->setCompletedSteps(completed_steps);
		emit operation->checkoutCurrentChanged(completed_steps);
	}

	if (total_steps != operation->totalSteps())
	{
		operation->setTotalSteps(total_steps);
		emit operation->checkoutTotalChanged(total_steps);
	}

	if (path != operation->checkoutPath())
	{
		operation->setCheckoutPath(path);
		emit operation->checkoutProgressChanged(completed_steps, total_steps, path);
	}
}
