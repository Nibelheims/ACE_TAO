/**
 * server for a reactor based connection establishment test using HTBP
 *
 * $Id$
 */

#include "ace/Log_Msg.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"

class Accept_Handler : public ACE_Event_Handler
{
public:
  Accept_Handler (ACE_SOCK_Acceptor& a);
  virtual ~Accept_Handler (void);
  virtual int handle_input (ACE_HANDLE );
private:
  ACE_SOCK_Acceptor& acceptor_;
  ACE::HTBP::Channel *channels_[2];
};

class Stream_Handler : public ACE_Event_Handler
{
public:
  Stream_Handler (ACE::HTBP::Stream &s);
  virtual ~Stream_Handler ();
  virtual int handle_input (ACE_HANDLE );
private:
  ACE::HTBP::Stream &stream_;
};


Accept_Handler::Accept_Handler(ACE_SOCK_Acceptor &a)
  :ACE_Event_Handler(),
   acceptor_(a)
{
  this->channels_[0] = this->channels_[1] = 0;
  if (this->reactor() == 0)
    this->reactor(ACE_Reactor::instance());
  this->reactor()->register_handler (acceptor_.get_handle(),
                                     this,
                                     ACE_Event_Handler::ACCEPT_MASK);
}

Accept_Handler::~Accept_Handler()
{
  this->reactor()->remove_handler (acceptor_.get_handle(),
                                   ACE_Event_Handler::ACCEPT_MASK|
                                   ACE_Event_Handler::DONT_CALL);
  acceptor_.close();
}

int
Accept_Handler::handle_input (ACE_HANDLE h)
{
  ACE::HTBP::Channel **ch = 0;
  if (h == acceptor_.get_handle())
    {
      ACE_SOCK_Stream *sock = new ACE_SOCK_Stream;
      acceptor_.accept(*sock);
      ch = channels_[0] == 0 ? &channels_[0] :& channels_[1];
      *ch = new ACE::HTBP::Channel(*sock);
      this->reactor()->register_handler (sock->get_handle(),
                                         this,
                                         ACE_Event_Handler::READ_MASK);
      return 0;
    }
  for (int i = 0; i < 2; i++)
    if (channels_[i] && channels_[i]->get_handle() == h)
      {
        ch = &channels_[i];
        break;
      }
  if (ch == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Accept_Handler::handle_input, unknown handle %d\n",h),
                      -1);
  int result = (*ch)->pre_recv();
  if (result == 0)
    {
      this->reactor()->remove_handler (h,
                                      ACE_Event_Handler::READ_MASK |
                                      ACE_Event_Handler::DONT_CALL);

      (*ch)->register_notifier(this->reactor());
      ACE::HTBP::Session *session = (*ch)->session();

      ACE::HTBP::Stream *stream = new ACE::HTBP::Stream(session);
      ACE_Event_Handler *handler = session->handler();

      if (handler == 0)
        {
          ACE_DEBUG ((LM_DEBUG,"Creating new stream handler for %d\n",
                      stream->get_handle()));
          Stream_Handler *sh = new Stream_Handler(*stream);
          session->handler (sh);
        }
      else
        ACE_DEBUG ((LM_DEBUG,"There is already a handler for %d\n",
                    stream->get_handle()));

      if ((*ch)->state() == ACE::HTBP::Channel::Data_Queued)
        {
          ACE_DEBUG ((LM_DEBUG,"Issuing notification on handler\n"));
          this->reactor()->notify (session->handler(),
                                   ACE_Event_Handler::READ_MASK);
        }

      *ch = 0;
    }
  return 0;
}

Stream_Handler::Stream_Handler (ACE::HTBP::Stream &s)
  :stream_(s)
{}
Stream_Handler::~Stream_Handler ()
{
}

int
Stream_Handler::handle_input (ACE_HANDLE h)
{
  char buffer[1000];
  ssize_t n = this->stream_.recv (buffer,1000);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Stream_Handler::handle_input %p\n","recv"),0);
  buffer[n] = 0;
  ACE_DEBUG ((LM_DEBUG,"Stream_Handler::handle_input (%d) read %d:\n%s\n",
              h,n,buffer));
  if (ACE_OS::strstr (buffer,"goodbye") != 0)
    this->reactor()->end_event_loop();
  else
    {
      ACE::HTBP::Channel *ch = stream_.session()->outbound();
      if (ch != 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Sending reply on %d\n",
                    ch->ace_stream().get_handle()));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Can't send reply on nul channel\n"));
      this->stream_.send ("Back atcha!",11);
    }
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_DEBUG ((LM_DEBUG,"At start of main\n"));
  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  ACE_INET_Addr local(8088);
  ACE_DEBUG ((LM_DEBUG,"got address\n"));
  ACE_SOCK_Acceptor acc(local,1);
  ACE_DEBUG ((LM_DEBUG,"opened listener\n"));

  Accept_Handler handler (acc);
  ACE_DEBUG ((LM_DEBUG,"server is ready\n"));

  ACE_Reactor::instance()->run_event_loop();
  return 0;
}
